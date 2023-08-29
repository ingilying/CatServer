add_rules("mode.debug", "mode.release")

package("mariadb-connector-cpp")
  add_urls("https://github.com/mariadb-corporation/mariadb-connector-cpp.git")
  add_deps("cmake")
  --add_deps("mariadb-connector-c")
  add_linkdirs("lib/mariadb")
  add_linkdirs("lib/mariadb/plugin")
 -- add_linkdirs("lib/mariadb-cpp")
 -- add_versions("1.1.2","e1f4c94aef10c38ee930cbeb97f7bc52dab0c2ed2b30bcde602585a2f2351ffc")
  on_load(function (package)
    if package:config("shared") then
      if package:is_plat("windows") then
        package:add("links","libmariadb")
      else
        package:add("links","mariadb")
      end
    else
      package:add("links","mariadb")
      package:add("links","mariadbclient")
      if package:is_plat("windows") then
        package:add("syslinks", "secur32", "shlwapi")
      end
    end
  --  package:add("links","mariadb")
    package:add("links","mariadbcpp")

  end)
  on_install("linux",function (package)
    os.exec("pwd")
 --   os.cd("source.temp/mariadb-connector-cpp-master")
    os.execv("git",{"submodule","init"})
    os.execv("git",{"submodule","update"})
    import("package.tools.cmake").install(package,configs)
    if package:config("shared") then
      os.trycp(path.join(package:installdir("lib"),"mariadb","*.dll"),package:installdir("bin"))
      os.trycp(path.join(package:installdir("lib"),"mariadb","*.so"),package:installdir("bin"))
      os.cp(path.join(package:installdir("lib"),"mariadb","plugin/*.so"),package:installdir("bin"))
    end
  end)
package_end()

add_requires("mariadb-connector-cpp")
add_requires("spdlog")
add_requires("workflow")
add_requires("protobuf-cpp")

target("catserver")
  set_kind("binary")
  set_languages("cxx17")
  set_rundir("$(projectdir)/")
  add_files("src/*.cpp")
  add_files("src/database/*.cpp")
  add_files("src/proto/*.cc")
  add_packages("mariadb-connector-cpp")
  add_packages("spdlog")
  add_packages("workflow")
  add_packages("protobuf-cpp")
--
-- If you want to known more usage about xmake, please see https://xmake.io
--
-- ## FAQ
--
-- You can enter the project directory firstly before building project.
--
--   $ cd projectdir
--
-- 1. How to build project?
--
--   $ xmake
--
-- 2. How to configure project?
--
--   $ xmake f -p [macosx|linux|iphoneos ..] -a [x86_64|i386|arm64 ..] -m [debug|release]
--
-- 3. Where is the build output directory?
--
--   The default output directory is `./build` and you can configure the output directory.
--
--   $ xmake f -o outputdir
--   $ xmake
--
-- 4. How to run and debug target after building project?
--
--   $ xmake run [targetname]
--   $ xmake run -d [targetname]
--
-- 5. How to install target to the system directory or other output directory?
--
--   $ xmake install
--   $ xmake install -o installdir
--
-- 6. Add some frequently-used compilation flags in xmake.lua
--
-- @code
--    -- add debug and release modes
--    add_rules("mode.debug", "mode.release")
--
--    -- add macro definition
--    add_defines("NDEBUG", "_GNU_SOURCE=1")
--
--    -- set warning all as error
--    set_warnings("all", "error")
--
--    -- set language: c99, c++11
--    set_languages("c99", "c++11")
--
--    -- set optimization: none, faster, fastest, smallest
--    set_optimize("fastest")
--
--    -- add include search directories
--    add_includedirs("/usr/include", "/usr/local/include")
--
--    -- add link libraries and search directories
--    add_links("tbox")
--    add_linkdirs("/usr/local/lib", "/usr/lib")
--
--    -- add system link libraries
--    add_syslinks("z", "pthread")
--
--    -- add compilation and link flags
--    add_cxflags("-stdnolib", "-fno-strict-aliasing")
--    add_ldflags("-L/usr/local/lib", "-lpthread", {force = true})
--
-- @endcode
--
