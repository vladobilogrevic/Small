#!/usr/bin/env python3
"""
TODO:
    -Fix VHDL w/ new features!
"""
import argparse
from datetime import datetime
from collections import defaultdict
from collections import namedtuple

Register = namedtuple('Register', ['name', 'is_external'])
Wire = namedtuple('Wire', ['name', 'rhs'])
Stage = namedtuple('Stage', ['name', 'reps'])


def parse_error(message, line_number, line_content):
    fmt = "Parse Error: {},\n\tline {}: \"{}\""
    raise SyntaxError(fmt.format(message, line_number+1, line_content.strip()))


def print_parse_summary(registers, wires, stages, patterns):
    print("*"*40)
    print("Begin Parse Summary")
    print("Found Registers:")
    for reg in registers:
        print("   {x.name:10} ext:{x.is_external}".format(x=reg))
    print("Found Wires:")
    for wire in wires:
        print("   {x.name:10} rhs:{x.rhs}".format(x=wire))
    print("Found Stages:")
    for i, stage in enumerate(stages):
        fmt = "{i}:   {x.name:10} len: {l:4} reps:{x.reps}"
        print(fmt.format(i=i, x=stage, l=len(patterns[(registers[0].name,
                                                       stage.name)])))
    print("End Parse Summary")
    print("*"*40)


def parse(lines):
    registers = []
    wires = []
    stages = []
    dev_name = "device"
    patterns = defaultdict(str)
    current_stage = None
    for i, line_orig in enumerate(lines):
        line = line_orig.split("#")[0].strip()
        if len(line) == 0:
            continue
        if line.startswith("dev"):
            toks = line.split()
            if len(toks) != 2:
                parse_error("Device definition", i, line_orig)
            dev_name = toks[1]
            continue
        if line.startswith("reg"):
            toks = line.split()
            if len(toks) == 3 and toks[1] == 'internal':
                print('Internal signal defined', toks[2])
                registers.append(Register(name=toks[2], is_external=False))
                continue
            elif len(toks) == 2:
                registers.append(Register(name=toks[1], is_external=True))
                continue
            else:
                parse_error("Register definition", i, line_orig)
        if line.startswith("wire"):
            toks = line.split(':')
            if len(toks) != 2:
                parse_error("Wire definition", i, line_orig)
            wires.append(Wire(name=toks[0].split()[1], rhs=toks[1].strip()))
            continue
        if line.startswith('stage'):
            toks = line.split()
            if len(toks) != 3:
                parse_error("Stage definition", i, line_orig)
            stage = Stage(name=toks[1], reps=int(toks[2]))
            stages.append(stage)
            current_stage = stage
            continue
        toks = [t.strip() for t in line.split(':')]
        if len(toks) != 2:
            parse_error("Pattern definition", i, line_orig)
        reg = toks[0]
        if reg not in [x.name for x in registers]:
            parse_error("Undefined signal: {}".format(reg), i, line_orig)
        pat = toks[1].replace(" ", "").replace("-", "1").replace("_", "0")
        if not set(pat).issubset(set(['0', '1'])):
            parse_error("Bad character", i, line_orig)
        patterns[(reg, current_stage.name)] += pat

    for stage in stages:
        n = len(patterns[(registers[0].name, stage.name)])
        for reg in registers:
            if len(patterns[(reg.name, stage.name)]) != n:
                fmt = "Uneven pattern length for signal:{}, stage:{}"
                raise SyntaxError(fmt.format(reg.name, stage.name))

    if ARGS.verbose:
        print_parse_summary(registers, wires, stages, patterns)
    return dev_name, registers, wires, stages, patterns

VHDL_STRINGS = {"header": [
                    "library ieee;",
                    "use ieee.std_logic_1164.all;",
                    "use ieee.numeric_std.all;",
                    "",
                    "ENTITY ***DEV_NAME*** IS",
                    "\tPORT(",
                    "\t\tclk : IN std_logic;"],
                "sig_out": "\t\t{} : OUT std_logic;",
                "res_in":  "\t\treset_gen : IN std_logic",
                "body_start": [
                    "\t);",
                    "END ***DEV_NAME***;",
                    "",
                    "ARCHITECTURE ***DEV_NAME***_arch OF ***DEV_NAME*** IS",
                    "SIGNAL counter:      integer := 0;",
                    "SIGNAL stage:        integer := 0;",
                    "SIGNAL stage_iter:   integer := 0;",
                    "BEGIN",
                    "\tPROCESS (clk)",
                    "\tBEGIN",
                    "\t\tIF(rising_edge(clk)) THEN",
                    "\t\t\tIF(reset_gen = '1') THEN",
                    "\t\t\t\tcounter <= 0;",
                    "\t\t\t\tstage <= 0;",
                    "\t\t\t\tstage_iter <= 0;",
                    "\t\t\tELSE"],
                "assign": "\t\t{} <= '{}';",
                "if_counter": "\tIF(counter = {}) THEN",
                "end_if": "\tEND IF;",
                "if_stage": "IF(stage = {}) THEN",
                "body_end1": "\tIF(counter = {}) THEN",
                "body_end2": "\t\tIF(stage_iter = {}) THEN",
                "body_end3": "\t\t\tstage <= (stage + 1) MOD {};",
                "body_end4": [
                    "\t\t\tstage_iter <= 0;",
                    "\t\tELSE",
                    "\t\t\tstage_iter <= stage_iter + 1;",
                    "\t\tEND IF;",
                    "\t\tcounter <= 0;",
                    "\tELSE",
                    "\t\tcounter <= counter + 1;",
                    "\tEND IF;",
                    "END IF;"],
                "footer": [
                    "\t\t\tEND IF;",
                    "\t\tEND IF;",
                    "\tEND PROCESS;",
                    "END ***DEV_NAME***_arch;"],
                "inner_indent": "\t\t\t\t"
                }

VERILOG_STRINGS = {"header": [
                       "`timescale 1 ns / 1 ps",
                       "",
                       "module ***DEV_NAME***",
                       "(",
                       "\tinput clk,"],
                   "sig_out_reg": "\toutput reg {},",
                   "sig_out_wire": "\toutput {},",
                   "res_in":  "\tinput reset_gen",
                   "header_end": [
                       ");",
                       "",
                       "reg [31:0]counter;",
                       "reg [7:0]stage;",
                       "reg [15:0]stage_iter;"],
                   "wire_def": "assign {}={};",
                   "reg_def": "reg {};",
                   "body_start": [
                       "always @(posedge clk ) begin",
                       "\tif(reset_gen == 1) begin",
                       "\t\tcounter <= 0;",
                       "\t\tstage <= 0;",
                       "\t\tstage_iter <= 0;",
                       "\tend",
                       "\telse begin"],
                   "assign": "\t\t{} <= {};",
                   "if_counter": "\tif(counter == {}) begin",
                   "end_if": "\tend",
                   "if_stage": "if(stage == {}) begin",
                   "body_end1": "\tif(counter == {}) begin",
                   "body_end2": "\t\tif(stage_iter == {}) begin",
                   "body_end3": "\t\t\tstage <= (stage + 1) % {};",
                   "body_end4": [
                       "\t\t\tstage_iter <= 0;",
                       "\t\tend",
                       "\t\telse begin",
                       "\t\t\tstage_iter <= stage_iter + 1;",
                       "\t\tend",
                       "\t\tcounter <= 0;",
                       "\tend",
                       "\telse begin",
                       "\t\tcounter <= counter + 1;",
                       "\tend",
                       "end"],
                   "footer": [
                       "\tend",
                       "end",
                       "endmodule"],
                   "inner_indent": "\t\t"
                   }


def make_hdl(dev_name, registers, wires, stages, patterns, hdl_strings):
    hdl = []
    hdl.extend(hdl_strings["header"])
    for sig in sorted(registers+wires, key=lambda x: x.name):
        if isinstance(sig, Register) and sig.is_external:
            hdl.append(hdl_strings["sig_out_reg"].format(sig.name))
        if isinstance(sig, Wire):
            hdl.append(hdl_strings["sig_out_wire"].format(sig.name))
    hdl.append(hdl_strings["res_in"])
    hdl.extend(hdl_strings["header_end"])
    for reg in registers:
        if not reg.is_external:
            hdl.append(hdl_strings['reg_def'].format(reg.name))
    for wire in wires:
        hdl.append(hdl_strings['wire_def'].format(wire.name, wire.rhs))
    hdl.extend(hdl_strings["body_start"])

    hdl_inner = []
    for stage_num, stage in enumerate(stages):
        stage_name, reps = stage
        stage_hdl = []
        n = len(patterns[(registers[0].name, stage.name)])
        for i in range(n):
            step_hdl = []
            for reg in registers:
                pattern = patterns[(reg.name, stage.name)]
                if i < 1 or pattern[i-1] != pattern[i]:
                    step_hdl.append(hdl_strings["assign"].format(reg.name,
                                                                 pattern[i]))
            if len(step_hdl) > 0:
                stage_hdl.append(hdl_strings["if_counter"].format(i))
                stage_hdl.extend(step_hdl)
                stage_hdl.append(hdl_strings["end_if"])
        if len(stage_hdl) > 0:
            hdl_inner.append(hdl_strings["if_stage"].format(stage_num))
            hdl_inner.extend(stage_hdl)
            hdl_inner.append(hdl_strings["body_end1"].format(n-1))
            hdl_inner.append(hdl_strings["body_end2"].format(reps-1))
            hdl_inner.append(hdl_strings["body_end3"].format(len(stages)))
            hdl_inner.extend(hdl_strings["body_end4"])
    hdl.extend([hdl_strings["inner_indent"]+x for x in hdl_inner])

    hdl.extend(hdl_strings["footer"])
    hdl = '\n'.join(hdl)
    hdl = hdl.replace("***DEV_NAME***", dev_name).replace("\t", "  ")

    return hdl


def main(args):
    with open(args.input, "r") as infile:
        lines = infile.readlines()

    if args.lang == "vhdl":
        hdl_strings = VHDL_STRINGS
        extension = ".vhd"
        comment = "--"
    else:
        hdl_strings = VERILOG_STRINGS
        extension = ".v"
        comment = "//"

    dev_name, registers, wires, stages, patterns = parse(lines)
    hdl = make_hdl(dev_name, registers, wires, stages, patterns, hdl_strings)

    if args.verbose:
        print("*"*40)
        print("BEGIN OUTPUT HDL")
        print("*"*40)
        print(hdl)
        print("*"*40)
        print("*"*40)

    outfile_name = ".".join(args.input.split('.')[:-1])+extension

    with open(outfile_name, "w") as outfile:
        outfile.write("{}Autogenerated {} on {}\n\n".format(comment,
                                                            args.lang,
                                                            datetime.now()))
        outfile.write(hdl)

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("input",
                        help="The input waveform file")
    parser.add_argument("lang", choices=("verilog", "vhdl"),
                        help="The HDL language for the output")
    parser.add_argument("-v", "--verbose", action="store_true",
                        help="Be Verbose")

    ARGS = parser.parse_args()
    main(ARGS)
