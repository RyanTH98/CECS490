file(REMOVE_RECURSE
  "bootloader.bin"
  "bootloader.map"
  "config/sdkconfig.cmake"
  "config/sdkconfig.h"
  "project_elf_src_esp32s2.c"
  "CMakeFiles/uf2-app"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/uf2-app.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
