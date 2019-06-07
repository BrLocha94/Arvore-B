@echo off 
IF EXIST %1 GOTO END 
MKDIR %1 
:END
