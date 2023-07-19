import chipwhisperer as cw

scope = cw.scope()
prog = cw.programmers.STM32FProgrammer
scope.default_setup()
cw.program_target(scope, prog, 'simpleserial-aes-CWLITEARM.hex')

scope.dis()
