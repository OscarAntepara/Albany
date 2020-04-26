set(OPTS
"-DALBANY_CTEST_TIMEOUT:INTEGER=60"
"-DALBANY_TRILINOS_DIR:FILEPATH=@lcm_install_dir@"
"-DCMAKE_CXX_FLAGS:STRING=\"@lcm_cxx_flags@\""
"-DCMAKE_VERBOSE_MAKEFILE:BOOL=OFF"
"-DENABLE_LANDICE:BOOL=ON"
"-DENABLE_CHECK_FPE:BOOL=@lcm_fpe_switch@"
"-DENABLE_FLUSH_DENORMALS:BOOL=@lcm_denormal_switch@"
"-DALBANY_ENABLE_FORTRAN:BOOL=OFF"
"@lcm_package_dir@"
)