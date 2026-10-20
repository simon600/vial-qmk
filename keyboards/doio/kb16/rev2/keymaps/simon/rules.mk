USER_NAME := simon

ENCODER_MAP_ENABLE = yes
VIA_ENABLE = yes
VIAL_ENABLE = yes
VIALRGB_ENABLE = yes

# Raw HID interface 1 (32 bytes, usage page 0xFF60): Vial.
# Interface 2 (64 bytes, usage page 0xFF61): kbd-daemon's lighting (OpenRGB
# packets) and host protocol. VIA's ids overlap OpenRGB's, so they can't share.
RAW_ENABLE = yes
RGB_RAW_ENABLE = yes
OPENRGB_ENABLE = yes
OPT_DEFS += -DOPENRGB_ENABLE
