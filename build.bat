@echo off
echo -- Building compiler --
msbuild regn.vcxproj -verbosity:minimal -nologo -property:Configuration=%1 -property:Platform=x64

