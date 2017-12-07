#!lua

solution "SampleClientGen"
   configurations { "Debug", "Release" }
   platforms { "x64", "x32" }
    
   project "SampleClientGen"
      kind "ConsoleApp"
      language "C++"
      files { "../**.h", "../**.cpp" }
      includedirs { "external/rapidjson", "../../../../../include" }
      flags { "Unicode" }
      buildoptions "-std=c++11 -stdlib=libc++"
      
      configuration "Debug"
         defines { "_DEBUG", "USE_WEBSOCKET", "VALIDATE_WAMP" }
         flags { "Symbols" }

      configuration "Release"
         defines { "NDEBUG", "USE_WEBSOCKET" }
         flags { "Optimize" }
