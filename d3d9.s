.macro loadnjmp sym
.global \sym\()_
.lcomm \sym\()_,8
.section .text
.global \sym
\sym:
call LoadDllStub
jmp *\sym\()_(%rip)
.endm

.section .text
LoadDllStub:
pushq %r9;pushq %r8;pushq %rdx;pushq %rcx
subq $0x30,%rsp
call LoadGenuineDll
addq $0x30,%rsp
popq %rcx;popq %rdx;popq %r8;popq %r9;ret

loadnjmp Ordinal16
loadnjmp Ordinal17
loadnjmp Ordinal18
loadnjmp Ordinal19
loadnjmp Ordinal20
loadnjmp Ordinal21
loadnjmp Ordinal22
loadnjmp Ordinal23
loadnjmp Direct3DShaderValidatorCreate9
loadnjmp PSGPError
loadnjmp PSGPSampleTexture
loadnjmp D3DPERF_BeginEvent
loadnjmp D3DPERF_EndEvent
loadnjmp D3DPERF_GetStatus
loadnjmp D3DPERF_QueryRepeatFrame
loadnjmp D3DPERF_SetMarker
loadnjmp D3DPERF_SetOptions
loadnjmp D3DPERF_SetRegion
loadnjmp DebugSetLevel
loadnjmp DebugSetMute
loadnjmp Direct3D9EnableMaximizedWindowedModeShim
loadnjmp Direct3DCreate9
loadnjmp Direct3DCreate9Ex
