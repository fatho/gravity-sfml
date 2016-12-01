# need gimp for automatically exporting the images
find_program(GIMP gimp)

set(SCRIPT_FU_IN )

if(GIMP_NOTFOUND)
  message(SEND_ERROR "Need GIMP to produce graphic assets.")
endif()

# need luca for checking
find_program(LUAC luac)

if(LUAC_NOTFOUND)
  message(WARNING "Need luac to check syntax of lua files.")
endif()

# Exports a GIMP XCF file to the given destination
function(data_file_gimp XCFFILE PNGFILE)
  # create temporary gimp script for
  set(SCRIPT_FILE "${PNGFILE}.script-fu")

  set(SCRIPT_FU "(let* ((image (car (gimp-file-load RUN-NONINTERACTIVE \"${XCFFILE}\" \"${XCFFILE}\")))
       (drawable (car (gimp-image-merge-visible-layers image CLIP-TO-IMAGE))))
      (file-png-save 1 image drawable \"${PNGFILE}\" \"${PNGFILE}\" 0 9 0 0 0 0 0 )
      (gimp-image-delete image)
      (gimp-quit 0))")
  message("    generating export script: ${SCRIPT_FILE}")
  file(GENERATE OUTPUT "${SCRIPT_FILE}" CONTENT "${SCRIPT_FU}")
  add_custom_command(OUTPUT ${PNGFILE} COMMAND "${GIMP}" -i -b - < "${SCRIPT_FILE}" MAIN_DEPENDENCY ${XCFFILE})
endfunction(data_file_gimp)

# Simply copies the data file to the destination
function(data_file_lua SOURCEFILE DESTFILE)
  add_custom_command(
    OUTPUT ${DESTFILE}
    COMMAND ${LUAC} -p ${SOURCEFILE}
    COMMAND ${CMAKE_COMMAND} -E copy ${SOURCEFILE} ${DESTFILE}
    MAIN_DEPENDENCY ${SOURCEFILE})
endfunction()

# Simply copies the data file to the destination
function(data_file_copy SOURCEFILE DESTFILE)
  add_custom_command(
    OUTPUT ${DESTFILE}
    COMMAND ${CMAKE_COMMAND} -E copy ${SOURCEFILE} ${DESTFILE}
    MAIN_DEPENDENCY ${SOURCEFILE})
endfunction()

# processes a single data file
function(process_data_file)
  set(options)
  set(oneValueArgs LIST_GENERATED FILE)
  set(multiValueArgs)
  cmake_parse_arguments(PDF "${options}" "${oneValueArgs}" "${multiValueArgs}" "${ARGN}")

  set(sourceFull ${CMAKE_CURRENT_SOURCE_DIR}/${PDF_FILE})

  get_filename_component(DataExt ${sourceFull} EXT)

  if(DataExt STREQUAL ".xcf")
    get_filename_component(ImgDir ${PDF_FILE} DIRECTORY)
    get_filename_component(ImgName ${PDF_FILE} NAME_WE)
    set(destFile ${ImgDir}/${ImgName}.png)
    set(destFull ${CMAKE_CURRENT_BINARY_DIR}/${ImgDir}/${ImgName}.png)
    data_file_gimp(${sourceFull} ${destFull})
  elseif(DataExt STREQUAL ".lua")
    set(destFile ${PDF_FILE})
    set(destFull ${CMAKE_CURRENT_BINARY_DIR}/${destFile})
    data_file_lua(${sourceFull} ${destFull})
  else()
    set(destFile ${PDF_FILE})
    set(destFull ${CMAKE_CURRENT_BINARY_DIR}/${destFile})
    data_file_copy(${sourceFull} ${destFull})
  endif()

  get_filename_component(DestSubDir ${destFile} DIRECTORY)
  install(FILES ${destFull} DESTINATION data/${DestSubDir})
  set(${PDF_LIST_GENERATED} ${${PDF_LIST_GENERATED}} ${destFull} PARENT_SCOPE)
endfunction()

# configures the build system for a mod
function(add_mod)
  set(options)
  set(oneValueArgs NAME)
  set(multiValueArgs FILES)
  cmake_parse_arguments(BUILD_MOD "${options}" "${oneValueArgs}" "${multiValueArgs}" "${ARGN}")

  set(GeneratedFiles)
  message("Processing data files:")
  foreach(DataFile ${BUILD_MOD_FILES})
    message("  - ${DataFile}")
    process_data_file(
      FILE ${DataFile}
      LIST_GENERATED GeneratedFiles)
  endforeach(DataFile)

  # add target for the mod
  add_custom_target(data_${BUILD_MOD_NAME} DEPENDS ${GeneratedFiles})
  add_dependencies(data data_${BUILD_MOD_NAME})
endfunction(add_mod)

# add target building all mods
add_custom_target(data ALL)

