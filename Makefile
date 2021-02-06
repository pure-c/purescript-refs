default: main
.PHONY: default

include $(PUREC_DIR)/mk/target.mk

main: .spago

main_CFLAGS = -g

$(eval $(call purs_mk_target,main,Test.Main,src test))

main_leakcheck: main
check: main_leakcheck
