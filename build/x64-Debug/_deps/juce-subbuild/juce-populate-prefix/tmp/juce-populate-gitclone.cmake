
if(NOT "D:/Practice Apps/AudioPluginsDev/Trempoline/build/x64-Debug/_deps/juce-subbuild/juce-populate-prefix/src/juce-populate-stamp/juce-populate-gitinfo.txt" IS_NEWER_THAN "D:/Practice Apps/AudioPluginsDev/Trempoline/build/x64-Debug/_deps/juce-subbuild/juce-populate-prefix/src/juce-populate-stamp/juce-populate-gitclone-lastrun.txt")
  message(STATUS "Avoiding repeated git clone, stamp file is up to date: 'D:/Practice Apps/AudioPluginsDev/Trempoline/build/x64-Debug/_deps/juce-subbuild/juce-populate-prefix/src/juce-populate-stamp/juce-populate-gitclone-lastrun.txt'")
  return()
endif()

execute_process(
  COMMAND ${CMAKE_COMMAND} -E rm -rf "D:/Practice Apps/AudioPluginsDev/Trempoline/build/x64-Debug/_deps/juce-src"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to remove directory: 'D:/Practice Apps/AudioPluginsDev/Trempoline/build/x64-Debug/_deps/juce-src'")
endif()

# try the clone 3 times in case there is an odd git clone issue
set(error_code 1)
set(number_of_tries 0)
while(error_code AND number_of_tries LESS 3)
  execute_process(
    COMMAND "D:/Secondary Programs/Git/cmd/git.exe"  clone --no-checkout --config "advice.detachedHead=false" "https://github.com/juce-framework/JUCE.git" "juce-src"
    WORKING_DIRECTORY "D:/Practice Apps/AudioPluginsDev/Trempoline/build/x64-Debug/_deps"
    RESULT_VARIABLE error_code
    )
  math(EXPR number_of_tries "${number_of_tries} + 1")
endwhile()
if(number_of_tries GREATER 1)
  message(STATUS "Had to git clone more than once:
          ${number_of_tries} times.")
endif()
if(error_code)
  message(FATAL_ERROR "Failed to clone repository: 'https://github.com/juce-framework/JUCE.git'")
endif()

execute_process(
  COMMAND "D:/Secondary Programs/Git/cmd/git.exe"  checkout master --
  WORKING_DIRECTORY "D:/Practice Apps/AudioPluginsDev/Trempoline/build/x64-Debug/_deps/juce-src"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to checkout tag: 'master'")
endif()

set(init_submodules TRUE)
if(init_submodules)
  execute_process(
    COMMAND "D:/Secondary Programs/Git/cmd/git.exe"  submodule update --recursive --init 
    WORKING_DIRECTORY "D:/Practice Apps/AudioPluginsDev/Trempoline/build/x64-Debug/_deps/juce-src"
    RESULT_VARIABLE error_code
    )
endif()
if(error_code)
  message(FATAL_ERROR "Failed to update submodules in: 'D:/Practice Apps/AudioPluginsDev/Trempoline/build/x64-Debug/_deps/juce-src'")
endif()

# Complete success, update the script-last-run stamp file:
#
execute_process(
  COMMAND ${CMAKE_COMMAND} -E copy
    "D:/Practice Apps/AudioPluginsDev/Trempoline/build/x64-Debug/_deps/juce-subbuild/juce-populate-prefix/src/juce-populate-stamp/juce-populate-gitinfo.txt"
    "D:/Practice Apps/AudioPluginsDev/Trempoline/build/x64-Debug/_deps/juce-subbuild/juce-populate-prefix/src/juce-populate-stamp/juce-populate-gitclone-lastrun.txt"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to copy script-last-run stamp file: 'D:/Practice Apps/AudioPluginsDev/Trempoline/build/x64-Debug/_deps/juce-subbuild/juce-populate-prefix/src/juce-populate-stamp/juce-populate-gitclone-lastrun.txt'")
endif()
