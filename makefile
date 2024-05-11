# ----------------------------
# Makefile Options
# ----------------------------

NAME = RAYCAST
ICON = icon.png
DESCRIPTION = "Raycasting Engine Experimental"
COMPRESSED = NO
ARCHIVED = NO

CFLAGS = -Wall -Wextra -Oz
CXXFLAGS = -Wall -Wextra -Oz

# ----------------------------

include $(shell cedev-config --makefile)
