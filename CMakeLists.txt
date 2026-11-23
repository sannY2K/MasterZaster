cmake_minimum_required(VERSION 3.22)

project(MasterZaster VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

option(MZ_BUILD_TESTS "Compilar los tests del nucleo (MZCore)" ON)
option(MZ_BUILD_STANDALONE "Compilar tambien la version Standalone (util para probar sin DAW)" OFF)
option(MZ_COPY_PLUGIN_AFTER_BUILD "Copiar el VST3 a la carpeta VST3 del sistema tras compilar (puede requerir admin)" OFF)

include(FetchContent)

# --- JUCE ---------------------------------------------------------------
FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG        8.0.12
    GIT_SHALLOW    ON
)
FetchContent_MakeAvailable(JUCE)

# --- Nucleo (sin JUCE, testeable en cualquier plataforma) ----------------
add_library(MZCore STATIC
    Source/Core/Dsp/Decibels.cpp
    Source/Core/Metering/LevelMeter.cpp
    Source/Core/Metering/StereoMeter.cpp
)
target_include_directories(MZCore PUBLIC Source/Core)
target_compile_features(MZCore PUBLIC cxx_std_20)

# --- Plugin ----------------------------------------------------------------
set(MZ_FORMATS VST3)
if(MZ_BUILD_STANDALONE)
    list(APPEND MZ_FORMATS Standalone)
endif()

juce_add_plugin(MasterZaster
    COMPANY_NAME                "MasterZaster"
    PLUGIN_MANUFACTURER_CODE    Mzst
    PLUGIN_CODE                 Mzv1
    FORMATS                     ${MZ_FORMATS}
    PRODUCT_NAME                "MasterZaster"
    IS_SYNTH                    FALSE
    NEEDS_MIDI_INPUT            FALSE
    NEEDS_MIDI_OUTPUT           FALSE
    IS_MIDI_EFFECT              FALSE
    VST3_CATEGORIES             Fx Analyzer
    COPY_PLUGIN_AFTER_BUILD     ${MZ_COPY_PLUGIN_AFTER_BUILD}
)

target_sources(MasterZaster PRIVATE
    Source/Plugin/PluginProcessor.cpp
    Source/Plugin/PluginProcessor.h
    Source/Plugin/PluginEditor.cpp
    Source/Plugin/PluginEditor.h
    Source/Plugin/UI/MZLookAndFeel.h
)

target_include_directories(MasterZaster PRIVATE Source)

target_compile_definitions(MasterZaster PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
)

target_link_libraries(MasterZaster
    PRIVATE
        MZCore
        juce::juce_audio_utils
        juce::juce_audio_processors
        juce::juce_dsp
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

# --- Tests -------------------------------------------------------------------
if(MZ_BUILD_TESTS)
    enable_testing()
    add_subdirectory(Tests)
endif()
