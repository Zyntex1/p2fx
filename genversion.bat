for /f "tokens=* usebackq" %%x in (`git describe --tags`) do (set version=%%x)
echo #define P2FX_VERSION "%version%">Version.hpp