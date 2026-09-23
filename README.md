## Rundfunk

Rundfunk is an audio degradation plugin created in C++ using the JUCE framework, utilizing CMake as the underlying build system. 
It introduces a series of distortion artifacts into the input audio signal via a simple implementation of an audio-rate modulation DSP algorithm.

The plugin is inspired by Ableton's Erosion audio effect, which itself uses audio-rate modulation to create a unique distorted, almost bit-crushed sound. 

More info on the Erosion effect can be found on the official Ableton Live manual under section 29.17: https://www.ableton.com/en/live-manual/12/live-audio-effect-reference/

Rundfunk can be built as a VST3, AU, AAX, along with various other targets. Thanks to CMake, the build targets can be easily configured in the CMakeLists.txt file via the FORMATS field in juce_add_plugin.
