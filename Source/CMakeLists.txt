project(Trempoline VERSION 0.0.1)

set (BaseTargetName Trempoline)

juce_add_plugin("${BaseTargetName}"
        VERSION "1.0"
        COMPANY_NAME "IH Pedals"
        IS_SYNTH FALSE
        NEEDS_MIDI_INPUT FALSE
        NEEDS_MIDI_OUTPUT FALSE
        IS_MIDI_EFFECT FALSE
        EDITOR_WANTS_KEYBOARD_FOCUS FALSE
        COPY_PLUGIN_AFTER_BUILD TRUE
        PLUGIN_MANUFACTURER_CODE IHpd
        PLUGIN_CODE Trmp
        FORMATS LV2 AU VST3 Standalone
        PRODUCT_NAME "Trempoline Tremolo")

juce_generate_juce_header(${BaseTargetName})

juce_add_binary_data("${BaseTargetName}Binaries"
    NAMESPACE assets
        SOURCES
            Assets/Images/big_sine.png
            Assets/Images/small_sine.png
            Assets/Images/triangle.png
    )

target_sources(${BaseTargetName} PRIVATE
        PluginProcessor.cpp
        PluginProcessor.h
        PluginEditor.cpp
        PluginEditor.h
        Parameters.h 
        
        UI/TrempolineLookAndFeel.h
        UI/TrempolineLookAndFeel.cpp
        UI/BigRotarySlider.h
        UI/BigRotarySlider.cpp
        UI/VerticalSlider.h
        UI/VerticalSlider.cpp
        UI/BypassButton.cpp
        
        DSP/Utils.h
        DSP/Tremolo.h
        DSP/Tremolo.cpp)

target_compile_definitions(${BaseTargetName}
        PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries(${BaseTargetName}
    PRIVATE
        "${BaseTargetName}Binaries"
        juce::juce_audio_utils
        juce::juce_dsp
        juce::juce_data_structures
        juce::juce_core
        juce::juce_graphics
        juce::juce_gui_basics
        juce::juce_gui_extra
        juce::juce_audio_processors
        juce::juce_audio_plugin_client
        juce::juce_audio_formats
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)
