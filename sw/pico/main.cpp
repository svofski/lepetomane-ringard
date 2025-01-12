#include <array>
#include <cstdlib>

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/binary_info.h"

#include "bsp/board.h"
#include "tusb.h"

#include "petomane.h"

enum  {
    BLINK_NOT_MOUNTED = 250,
    BLINK_MOUNTED = 1000,
    BLINK_SUSPENDED = 2500,
};

Petomane petomane;

static uint32_t blink_interval_ms = BLINK_NOT_MOUNTED;
const uint LED_PIN = PICO_DEFAULT_LED_PIN;

void led_blinking_task(void);
void midi_task(void);

struct DebugMonitor
{
    Petomane& petomane;
    std::array<char, 64> cmdbuf;
    int bufindex;
    int wrn = 0;

    DebugMonitor(Petomane& p): petomane(p), bufindex(0) {}

    void task()
    {
        int c = getchar_timeout_us(10);
        if (c >= 0 && c <= 255) {
            if (c == '\r') {
                c = '\n';
            }
            putchar(c);
            if (c == '\n' || bufindex >= cmdbuf.size() - 1) {
                c = 0;
            }
            cmdbuf[bufindex++] = c;
            if (c == 0) {
                if (bufindex > 1) {
                    process_cmd();
                }
                bufindex = 0;
            }
        }
    }

    void process_cmd()
    {
        int result = 0;
        switch (cmdbuf[0]) {
            case 'd':
                result = process_div();
                break;
            case 'b':
                result = process_bus();
                break;
            case 'w':
                gpio_put(15, wrn);
                wrn ^= 1;
                break;
            default:
                printf("unknown command: [%s]\n", &cmdbuf[0]);
                break;
        }

        if (result == 0) {
            printf("OK\n");
        }
        else {
            printf("ERROR\n");
        }
    }

    void skip_whitespace(int& pos)
    {
        while ((cmdbuf[pos] == ' ' || cmdbuf[pos] == '\t') && pos < cmdbuf.size()) ++pos;
    }

    // d 0 1234 -- sets chan 0 divider to 1234 decimal
    int process_div()
    {
        int pos = 1;

        // first arg
        skip_whitespace(pos);
        if (pos >= cmdbuf.size()) return -1;
        int chan = cmdbuf[pos] - '0';
        if (chan < 0 || chan > 2) return -1;

        // second arg
        ++pos; 
        skip_whitespace(pos);
        if (pos >= cmdbuf.size()) return -1;

        int div = atoi(&cmdbuf[pos]);
        if (div < 0 || div > 65535) return -1;

        printf("set div[%d]=%d\n", chan, div);
        petomane.set_enabled(chan, true);
        petomane.set_div(chan, div);

        return 0;
    }

    // set bus
    // b a b -- abus = a, dbus = b
    // 0-3 = PPI
    // 4-7 = TIMER
    // 8-9 = MVOL
    int process_bus()
    {
        int pos = 1;

        // first arg
        skip_whitespace(pos);
        if (pos >= cmdbuf.size()) return -1;
        int a = cmdbuf[pos] - '0';
        if (a < 0 || a > 9) return -1;

        // second arg
        ++pos; 
        skip_whitespace(pos);
        if (pos >= cmdbuf.size()) return -1;

        uint8_t dbus = atoi(&cmdbuf[pos]) & 0xff;

        int dev = (a & 0xc) >> 2;
        int reg = a & 3;
        printf("set bus dev=%d reg=%d D=%02x\n", dev, reg, dbus);
        petomane.bsr_dev(dev, reg, 0, dbus);

        return 0;
    }
};

DebugMonitor debug_monitor(petomane);

int main() {
    board_init();
    stdio_init_all();
    petomane.board_init();

    tud_init(BOARD_TUD_RHPORT);

    printf("lepetomane-ringard\n");

    while (1)
    {
        tud_task(); // tinyusb device task
        led_blinking_task();
        //midi_task();
        debug_monitor.task();
    }
}

//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

// Invoked when device is mounted
void tud_mount_cb(void)
{
    blink_interval_ms = BLINK_MOUNTED;
}

// Invoked when device is unmounted
void tud_umount_cb(void)
{
    blink_interval_ms = BLINK_NOT_MOUNTED;
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en)
{
    (void) remote_wakeup_en;
    blink_interval_ms = BLINK_SUSPENDED;
}

// Invoked when usb bus is resumed
void tud_resume_cb(void)
{
    blink_interval_ms = BLINK_MOUNTED;
}

//--------------------------------------------------------------------+
// MIDI Task
//--------------------------------------------------------------------+

// Variable that holds the current position in the sequence.
uint32_t note_pos = 0;

// Store example melody as an array of note values
uint8_t note_sequence[] =
{
    74,78,81,86,90,93,98,102,57,61,66,69,73,78,81,85,88,92,97,100,97,92,88,85,81,78,
    74,69,66,62,57,62,66,69,74,78,81,86,90,93,97,102,97,93,90,85,81,78,73,68,64,61,
    56,61,64,68,74,78,81,86,90,93,98,102
};

void midi_task(void)
{
    static uint32_t start_ms = 0;
    uint8_t msg[3];

    // send note every 1000 ms
    if (board_millis() - start_ms < 286) return; // not enough time
    start_ms += 286;

    // Previous positions in the note sequence.
    int previous = note_pos - 1;

    // If we currently are at position 0, set the
    // previous position to the last note in the sequence.
    if (previous < 0) previous = sizeof(note_sequence) - 1;

    // Send Note On for current position at full velocity (127) on channel 1.
    msg[0] = 0x90;                    // Note On - Channel 1
    msg[1] = note_sequence[note_pos]; // Note Number
    msg[2] = 127;                     // Velocity
    tud_midi_n_stream_write(0, 0, msg, 3);

    printf("note_on: %02x %02x %02x\n", msg[0], msg[1], msg[2]);

    // Send Note Off for previous note.
    msg[0] = 0x80;                    // Note Off - Channel 1
    msg[1] = note_sequence[previous]; // Note Number
    msg[2] = 0;                       // Velocity
    tud_midi_n_stream_write(0, 0, msg, 3);

    // Increment position
    note_pos++;

    // If we are at the end of the sequence, start over.
    if (note_pos >= sizeof(note_sequence)) note_pos = 0;
}

//--------------------------------------------------------------------+
// BLINKING TASK
//--------------------------------------------------------------------+
void led_blinking_task(void)
{
    static uint32_t start_ms = 0;
    static bool led_state = false;

    // Blink every interval ms
    if ( board_millis() - start_ms < blink_interval_ms) return; // not enough time
    start_ms += blink_interval_ms;

    board_led_write(led_state);
    led_state = 1 - led_state; // toggle
}
