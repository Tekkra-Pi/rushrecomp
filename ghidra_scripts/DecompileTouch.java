//Decompile touch functions - write output to file
// @category Analysis

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.address.Address;
import ghidra.app.cmd.disassemble.DisassembleCommand;
import java.io.*;

public class DecompileTouch extends GhidraScript {
    
    private static final String[][] TOUCH_FUNCTIONS = {
        {"0x02033c60", "TouchState_Dispatcher"},
        {"0x0203387c", "TouchState_AlternateUpdater"},
        {"0x02033a8c", "TouchState_MainUpdater"},
        {"0x02009998", "Sampler_ConsumeSharedSamples"},
        {"0x02033650", "TouchState_Producer"},
        {"0x02033de8", "TouchState_Init"},
        {"0x02009908", "InputSubsys_Init"},
        {"0x02009544", "Controller_GetIndex"},
        {"0x020091a4", "Controller_CopySampleSlot"},
        {"0x020095f8", "Controller_CopyHalfwords"},
        {"0x02009650", "Controller_PollCapture"},
        {"0x020096f0", "Controller_SetCallback"},
        {"0x0200918c", "Controller_WaitFlag"},
        {"0x02009178", "Controller_CheckFlag"},
        {"0x0200985c", "Controller_Unknown85c"},
        {"0x02009714", "Controller_Unknown714"},
        {"0x0203363c", "TouchState_DispatcherFull"},
        {"0x02033cac", "TouchState_Disable"},
        {"0x02033d40", "TouchState_ClearEnable"},
        {"0x02033d60", "TouchState_ResetEnable"},
    };
    
    @Override
    public void run() throws Exception {
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);
        
        String outputPath = System.getenv().containsKey("GHIDRA_OUTPUT")
            ? System.getenv("GHIDRA_OUTPUT")
            : System.getProperty("user.home") + "/sonic-rush-decompiled.txt";
        PrintWriter writer = new PrintWriter(new FileWriter(outputPath));
        
        writer.println("========================================================");
        writer.println("DECOMPILE TOUCH FUNCTIONS - ARM9");
        writer.println("Program: " + currentProgram.getName());
        writer.println("========================================================");
        
        for (String[] pair : TOUCH_FUNCTIONS) {
            String addrStr = pair[0];
            String name = pair[1];
            
            writer.println("\n=== " + name + " (" + addrStr + ") ===");
            println("Processing: " + name + " (" + addrStr + ")");
            
            try {
                Address addr = currentProgram.getAddressFactory().getAddress(addrStr);
                
                Instruction instr = currentProgram.getListing().getInstructionAt(addr);
                if (instr == null) {
                    DisassembleCommand cmd = new DisassembleCommand(addr, null, true);
                    cmd.applyTo(currentProgram);
                    instr = currentProgram.getListing().getInstructionAt(addr);
                    if (instr == null) {
                        writer.println("  NO INSTRUCTION at " + addrStr);
                        continue;
                    }
                }
                
                Function func = currentProgram.getFunctionManager().getFunctionAt(addr);
                if (func == null) {
                    func = currentProgram.getFunctionManager().getFunctionContaining(addr);
                }
                
                if (func == null) {
                    writer.println("  NO FUNCTION - raw disassembly:");
                    Address end = addr.add(0xC0);
                    for (Instruction i = currentProgram.getListing().getInstructionAt(addr);
                         i != null && i.getAddress().compareTo(end) < 0;
                         i = currentProgram.getListing().getInstructionAt(i.getAddress().add(i.getLength()))) {
                        writer.println("    " + i);
                    }
                    continue;
                }
                
                writer.println("  Function: " + func.getName() + " @ " + func.getEntryPoint());
                writer.println("  Size: " + func.getBody().getNumAddresses() + " bytes");
                
                DecompileResults result = decomp.decompileFunction(func, 60, getMonitor());
                if (result != null && result.getDecompiledFunction() != null) {
                    String cCode = result.getDecompiledFunction().getC();
                    if (cCode != null && cCode.length() > 0) {
                        writer.println("  DECOMPILED:");
                        writer.println(cCode);
                    } else {
                        writer.println("  DECOMPILATION EMPTY");
                    }
                } else {
                    writer.println("  DECOMPILATION FAILED");
                }
            } catch (Exception e) {
                writer.println("  ERROR: " + e.getMessage());
            }
        }
        
        writer.println("\n========================================================");
        writer.println("DONE");
        writer.println("========================================================");
        writer.close();
        decomp.dispose();
        
        println("Output written to " + outputPath);
    }
}
