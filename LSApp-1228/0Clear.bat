@echo off
del *.bak /s
del *.obj /s
del *.idb /s
del *.sdf /s
del *.ilk /s
del *.pch /s
del *.pdb /s
del *.aps /s
del *.tlog /s
del *.opensdf /s /a:h
del *.VC.db /s
del *.v12.suo /s /a:h
del *.vcxproj.user /s
del *.CppClean.log /s
rd ipch /s /Q
rd .vs /s /Q
pause
