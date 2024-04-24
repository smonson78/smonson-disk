TARGETS=hdd tb 

all: $(TARGETS)
clean:
	$(RM) $(TARGETS) dump.lxt

hdd: hdd.v
	iverilog -o $@ $^

tb: tb.v hdd.v
	iverilog -o $@ $^

%: %.v
	iverilog -o $@ $^

wave: tb
	vvp tb -lxt2

