import os
import scons.utils
import scons.checks

def isQuiet():
    import sys
    return '-s' in sys.argv

def isPlatform(platform):
    import sys
    return platform in sys.platform

def isWindows():
    return isPlatform("win32")

def isOSX104():
    import platform
    return isPlatform("darwin") and platform.processor() == 'powerpc'

# Assume 10.6 and up
def isOSX():
    return isPlatform("darwin") and not isOSX104()

def file_exists(path):
    return os.path.exists(path)

def makeUseEnvironment(key, default):
    def use():
        import os
        try:
            return int(os.environ[key]) == 1
        except KeyError:
            return default
    return use

def makeUseArgument(key, default):
    def use():
        try:
            return int(ARGUMENTS[key]) == 1
        except KeyError:
            return default
    return use

# Functions made with `makeUseArgument' should be set by command line arguments
# to scons:
# $ scons gch=1
# If `makeUseEnvironment' is used instead then a shell variable should be set:
# $ export prx=1
# $ scons
usePrx = makeUseEnvironment('prx', False)
isVerbose = makeUseArgument('verbose', False)
useIntel = makeUseEnvironment('intel', False)
useMinpspw = makeUseEnvironment('minpspw', False)
useAndroid = makeUseEnvironment('android', False)
useAndroidX86 = makeUseEnvironment('androidx86', False)
useIos = makeUseEnvironment('ios', False)
usePs3 = makeUseEnvironment('ps3', False)
useNDS = makeUseEnvironment('nds', False)
useDingoo = makeUseEnvironment('dingoo', False)
useXenon = makeUseEnvironment('xenon', False)
usePandora = makeUseEnvironment('pandora', False)
useWii = makeUseEnvironment('wii', False)
useLLVM = makeUseEnvironment('llvm', False)
useNacl = makeUseEnvironment('nacl', False)
useMad = makeUseEnvironment('mad', False)
useGCW = makeUseEnvironment('gcw', False)
nativeCompile = makeUseEnvironment('native', False)
enableProfiled = makeUseEnvironment('PROFILE', False)
showTiming = makeUseEnvironment('timing', False)

def ps3devPath():
    try:
        return os.environ['PS3DEV']
    except KeyError:
        return '/opt/ps3dev'

def wrapSymbols(env):
    wrapped_symbols = Split("""open read close lseek lstat access""")
    def wrap(symbol):
        return '-Wl,--wrap=%s' % symbol
    env.Append(LINKFLAGS = map(wrap, wrapped_symbols))

def checkAllegro(context):
    context.Message("Checking for Allegro 4... ")

    def testAllegro(context):
        return context.TryLink("""
            #include <allegro.h>
            int main(int argc, char ** argv){
              install_allegro(0, NULL, NULL);
              return 0;
            }
            END_OF_MAIN()
        """, ".c")

    # use pkg-config
    def allegro44(context):
        tmp = context.env.Clone()
        env = context.env
        ok = 1
        try:
            scons.utils.safeParseConfig(env, 'pkg-config allegro --cflags --libs')
            env.Append(CPPDEFINES = ['USE_ALLEGRO'])
            ok = testAllegro(context)
        except OSError:
            ok = 0 

        if not ok:
            context.sconf.env = tmp
        else:
            context.Message('found 4.4')

        return ok

    # use allegro-config
    def allegro42(context):
        tmp = context.env.Clone()
        env = context.env
        ok = 1
        try:
            def enableAllegro(env2):
                scons.utils.safeParseConfig(env2, 'allegro-config --cflags --libs')
                env2.Append(CPPDEFINES = ['USE_ALLEGRO'])
            scons.utils.safeParseConfig(env, 'allegro-config --cflags --libs')
            env['paintown_enableAllegro'] = enableAllegro
            env.Append(CPPDEFINES = ['USE_ALLEGRO'])
            ok = testAllegro(context)
        except OSError:
            ok = 0 

        if not ok:
            context.sconf.env = tmp
        else:
            context.Message('found 4.2')

        return ok

    ok = allegro44(context) or allegro42(context)

    context.Result(scons.utils.colorResult(ok))
    return ok

# find freetype in windows since we dont have freetype-config
def checkWindowsFreeType(context):
    context.Message("Checking for Freetype... ")
    def build():
        return context.TryCompile("""
#include <ft2build.h>
#include FT_FREETYPE_H
int main(int argc, char ** argv){
    int major = FREETYPE_MAJOR; 
    return 0;
}
""", ".c")
    if not build():
        if useMingw():
            import os
            tmp = context.env.Clone()
            mingw = os.environ['MINGDIR']
            context.env.Append(CPPPATH = ["%s/include/freetype2" % mingw])
            if not build():
                context.env = tmp
                context.Result(scons.utils.colorResult(0))
                return 0
        else:
            context.Message("don't know how to find freetype for a non-mingw compiler")
            context.Result(scons.utils.colorResult(0))
            return 0

    context.Result(scons.utils.colorResult(1))
    return 1

def isCygwin():
    try:
        return os.environ['CYGWIN'] == '1'
    except:
        return False    

def useMingw():
    try:
        return "mingw" in ARGUMENTS[ 'env' ]
    except KeyError:
        return False

def readExec( program ):
    try:
        return os.popen(program).readline().replace("\n",'')
    except OSError:
        return ""

def useDistcc():
    try:
        return int(os.environ['USE_DISTCC']) == 1
    except KeyError:
        return False
        
def getDebug():
    try:
        return int(os.environ[ 'DEBUG' ])
    except KeyError:
        return 0

def less_verbose(env):
    link_color = 'light-red'
    ar_color = 'yellow'
    ranlib_color = 'light-purple'
    peg_color = 'light-cyan'
    env['CCCOMSTR'] = "%s %s" % (scons.utils.colorize('Compiling c file', 'light-green'), scons.utils.colorize('$SOURCE', 'light-blue'))
    env['SHCCCOMSTR'] = "%s %s" % (scons.utils.colorize('Compiling c file', 'light-green'), scons.utils.colorize('$SOURCE', 'light-blue'))
    env['CXXCOMSTR'] = "%s %s" % (scons.utils.colorize('Compiling c++ file', 'light-green'), scons.utils.colorize('$SOURCE', 'light-blue'))
    env['SHCXXCOMSTR'] = "%s %s" % (scons.utils.colorize('Compiling c++ file', 'light-green'), scons.utils.colorize('$SOURCE', 'light-blue'))
    env['LINKCOMSTR'] = "%s %s" % (scons.utils.colorize('Linking', link_color), scons.utils.colorize('$TARGET', 'light-blue'))
    env['SHLINKCOMSTR'] = "%s %s" % (scons.utils.colorize('Linking', link_color), scons.utils.colorize('$TARGET', 'light-blue'))
    env['ARCOMSTR'] = "%s %s" % (scons.utils.colorize('Building library', ar_color), scons.utils.colorize('$TARGET', 'light-blue'))
    env['RANLIBCOMSTR'] = "%s %s" % (scons.utils.colorize('Indexing library', ranlib_color), scons.utils.colorize('$TARGET', 'light-blue'))
    env['PEG_MAKE'] = "%s %s" % (scons.utils.colorize('Creating peg parser', peg_color), scons.utils.colorize('$TARGET', 'light-blue'))
    return env

def getEnvironment(debug):
    def intel(env):
        env['CC'] = 'icc'
        env['CXX'] = 'icpc'
        # '-Werror-all', '-Weffc++'
        # TODO: replace these flags with descriptive names
        env.Append(CCFLAGS = ['-wd981', '-wd2259'],
                   CXXFLAGS = ['-wd981', '-wd271',
                               '-wd383', '-wd869',
                               '-wd1599'])
        return env

    def xenon(env):
        print "Environment is Xenon"
        xenon = "/usr/local/xenon"
        prefix = "xenon-"
        def setup(pre, x):
            return '%s%s' % (pre, x)
        env['CC'] = setup(prefix, 'gcc')
        env['LD'] = setup(prefix, 'ld')
        env['CXX'] = setup(prefix, 'g++')
        env['AS'] = setup(prefix, 'as')
        env['AR'] = setup(prefix, 'ar')
        env['OBJCOPY'] = setup(prefix, 'objcopy')
        env.Append(LIBS = Split("""xenon m fat"""))
        # env.Append(CCFLAGS = Split("""-m32 -mcpu=cell -mtune=cell -mpowerpc64 -mhard-float -ffunction-sections -fdata-sections -maltivec -fno-pic"""))
        env.Append(CCFLAGS = Split("""-m32 -mcpu=cell -mtune=cell -mpowerpc64 -mhard-float -maltivec -fno-pic"""))
        env.Append(CPPDEFINES = ['XENON'])
        env.Append(CPPPATH = ['%(xenon)s/usr/include' % {'xenon': xenon}])
        env.Append(LIBPATH = ['%(xenon)s/xenon/lib/32' % {'xenon': xenon}])
        env.Append(LIBPATH = ['%(xenon)s/usr/lib' % {'xenon': xenon}])
        env.Append(LINKFLAGS = Split("""-m32 -maltivec -mpowerpc64 -mhard-float -fno-pic -n -T %(xenon)s/app.lds""" % {'xenon': xenon}))
        env['LINKCOM'] = '$CXX $LINKFLAGS $SOURCES -Wl,--start-group $ARCHIVES $_LIBDIRFLAGS $_LIBFLAGS -Wl,--end-group -o $TARGET'
        env.PrependENVPath('PATH', setup(xenon, '/bin'))
        env.PrependENVPath('PATH', setup(xenon, '/usr/bin'))
        return env
    def dingux(env):
        import os
        print "Environment is Dingux"
        mips = '/opt/mipsel-linux-uclibc'
        bin_path = mips + '/usr/bin'
        prefix = 'mipsel-linux-'
        def setup(pre, x):
            return '%s%s' % (pre, x)
        env['CC'] = setup(prefix, 'gcc')
        env['LD'] = setup(prefix, 'ld')
        env['CXX'] = setup(prefix, 'g++')
        env['AS'] = setup(prefix, 'as')
        env['AR'] = setup(prefix, 'ar')
        env['OBJCOPY'] = setup(prefix, 'objcopy')
        env.Append(CPPDEFINES = ['DINGOO'])
        env['LINKCOM'] = '$CXX $LINKFLAGS $SOURCES -Wl,--start-group $ARCHIVES $_LIBDIRFLAGS $_LIBFLAGS -Wl,--end-group -o $TARGET'
        # env.Append(CPPDEFINES = ['DINGOO', 'LINUX', 'MPU_JZ4740'])
        #env.Append(CPPPATH = [setup(dingoo_path, "/include"),
        #                      setup(dingoo_path, "/include/SDL"),
        #                      setup(dingoo_path, "/include/freetype2")])
        #env.Append(LIBPATH = [setup(dingoo_path, '/lib')])
        #flags = ['-G0', '-mips32', '-mno-abicalls', '-msoft-float', '-fno-builtin', '-fno-pic']
        #env.Append(CCFLAGS = flags)
        #env.Append(CXXFLAGS = flags)
        #env.Append(LINKFLAGS = ['-nodefaultlibs', '%s/lib/dingoo.xn' % dingoo_path])
        # env.Append(LIBS = ['c', 'm', 'fgl', 'sml', 'jz4740', 'gcc'])
        #env.Append(LIBS = ['SDL', 'jz4740', 'sml', 'fgl', 'c', 'gcc'])
        #env['LINKCOM'] = '$LD $LINKFLAGS $SOURCES $_LIBDIRFLAGS $_LIBFLAGS -o $TARGET'
        env.PrependENVPath('PATH', bin_path)
        # env.PrependENVPath('PATH', '%s/bin' % dingoo_path)
        return env

    def dingoo(env):
        import os
        print "Environment is Dingoo"
        dingoo_path = os.environ['DINGOO_SDK']
        mips_path = os.environ['MIPSTOOLS']
        bin_path = mips_path + '/bin'
        prefix = 'mipsel-linux-'
        def setup(pre, x):
            return '%s%s' % (pre, x)
        env['CC'] = setup(prefix, 'gcc')
        env['LD'] = setup(prefix, 'ld')
        env['CXX'] = setup(prefix, 'g++')
        env['AS'] = setup(prefix, 'as')
        env['AR'] = setup(prefix, 'ar')
        env['OBJCOPY'] = setup(prefix, 'objcopy')
        env.Append(CPPDEFINES = ['DINGOO', 'LINUX', 'MPU_JZ4740'])
        env.Append(CPPPATH = [setup(dingoo_path, "/include"),
                              setup(dingoo_path, "/include/SDL"),
                              setup(dingoo_path, "/include/freetype2")])
        env.Append(LIBPATH = [setup(dingoo_path, '/lib')])
        flags = ['-G0', '-mips32', '-mno-abicalls', '-msoft-float', '-fno-builtin', '-fno-pic']
        env.Append(CCFLAGS = flags)
        env.Append(CXXFLAGS = flags)
        env.Append(LINKFLAGS = ['-nodefaultlibs', '%s/lib/dingoo.xn' % dingoo_path])
        # env.Append(LIBS = ['c', 'm', 'fgl', 'sml', 'jz4740', 'gcc'])
        env.Append(LIBS = ['SDL', 'jz4740', 'sml', 'fgl', 'c', 'gcc'])
        env['LINKCOM'] = '$LD $LINKFLAGS $SOURCES $ARCHIVES $_LIBDIRFLAGS $_LIBFLAGS -o $TARGET'
        env.PrependENVPath('PATH', bin_path)
        env.PrependENVPath('PATH', '%s/bin' % dingoo_path)
        env['LINKCOM'] = '$CXX $LINKFLAGS $SOURCES -Wl,--start-group $ARCHIVES $_LIBDIRFLAGS $_LIBFLAGS -Wl,--end-group -o $TARGET'
        return env

    def pandora(env):
        import os
        print "Environment is Pandora"
        env['CC'] = os.environ['CC']
        env['CXX'] = os.environ['CXX']
        env['LD'] = os.environ['CC']
        env['AS'] = os.environ['AS']
        env['AR'] = os.environ['AR']
        env.Append(CPPDEFINES = ['PANDORA'])
        flags = ['-O3', '-pipe', '-march=armv7-a', '-mtune=cortex-a8', '-mfpu=neon', '-mfloat-abi=softfp', '-ftree-vectorize', '-ffast-math', '-fsingle-precision-constant']
        env.Append(CCFLAGS = flags)
        env.Append(CXXFLAGS = flags)
        env['LINKCOM'] = '$CXX $LINKFLAGS $SOURCES -Wl,--start-group $ARCHIVES $_LIBDIRFLAGS $_LIBFLAGS -Wl,--end-group -o $TARGET'
        return env

    def nds(env):
        print "Environment is nds"
        path = '/opt/devkitARM'
        bin_path = path + '/bin'
        # libexec_path = path + '/libexec/gcc/arm-eabi/4.5.1'
        prefix = 'arm-eabi-'
        def setup(pre, x):
            return '%s%s' % (pre, x)
        env['CC'] = setup(prefix, 'gcc')
        env['LD'] = setup(prefix, 'ld')
        env['CXX'] = setup(prefix, 'g++')
        env['AS'] = setup(prefix, 'as')
        env['AR'] = setup(prefix, 'ar')
        env['OBJCOPY'] = setup(prefix, 'objcopy')

        env.Append(CPPPATH = [setup(path, "/libnds/include"),
                              setup(path, "/libnds/include/SDL"),
                              setup(path, "/libnds/include/freetype2")])

        env.Append(LIBPATH = [setup(path, '/libnds/lib')])
        env.Append(CPPDEFINES = ['NDS'])
        env['LINKCOM'] = '$CXX $LINKFLAGS $SOURCES -Wl,--start-group $ARCHIVES $_LIBDIRFLAGS $_LIBFLAGS -Wl,--end-group -o $TARGET'
        env.PrependENVPath('PATH', bin_path)
        # env.PrependENVPath('PATH', libexec_path)
        return env

    # minpspw for psp dev environment on windows (and linux?)
    def minpspw(env):
        print "Environment is minpspw (psp development)"
        # on linux, symlink the pspsdk to /opt/pspsdk, or just put it there
        path = '/opt/pspsdk'
        try:
            path = os.environ['MINPSPWDIR']
        except KeyError:
            pass
        bin_path = path + '/bin/'
        prefix = 'psp-'
        def setup(pre, x):
            return '%s%s' % (pre, x)
        env['CC'] = setup(prefix, 'gcc')
        env['LD'] = setup(prefix, 'ld')
        env['CXX'] = setup(prefix, 'g++')
        env['AS'] = setup(prefix, 'as')
        env['AR'] = setup(prefix, 'ar')
        env['OBJCOPY'] = setup(prefix, 'objcopy')
        # FIXME: try to use freetype-config and sdl-config to find these paths
        # instead of hard coding them
        env.Append(CPPPATH = [setup(path, "/psp/include"),
                              setup(path, "/psp/include/SDL"),
                              setup(path, "/psp/include/freetype2"),
                              setup(path, "/psp/sdk/include")])
        env.Append(CPPDEFINES = ['MINPSPW','_PSP_FW_VERSION=150'])
        env.Append(LIBPATH = [setup(path, '/psp/lib'),
                              setup(path, '/psp/sdk/lib')])
        if usePrx():
            env.Append(LINKFLAGS = ['-specs=%s/psp/sdk/lib/prxspecs' % path,
                                    '-Wl,-q,-T%s/psp/sdk/lib/linkfile.prx' % path])
        # So that xmain.cpp can pick up on windows requirement
        if isWindows():
            env.Append(CPPDEFINES = ['INCLUDE_SCE_MODULE_INFO'])
        flags = ['-G0', '-fexceptions']
        env.Append(CCFLAGS = flags)
        env.Append(CXXFLAGS = flags)
        env['LINKCOM'] = '$CC $LINKFLAGS -Wl,--start-group $ARCHIVES $SOURCES $_LIBDIRFLAGS $_LIBFLAGS -Wl,--end-group -o $TARGET'
        env.Append(LINKFLAGS = flags)
# pthread-psp
        all = Split("""
SDL
SDL_image
SDL_mixer
SDLmain
pspdebug
ogg
vorbis
vorbisfile
GL
stdc++
m
freetype
png
z
jpeg
c
pspctrl
pspvfpu
pspdisplay
psphprm
pspaudio
pspgu
pspge
psprtc
pspsdk
pspuser
pspkernel
pspnet_inet
""")
        env.Append(LIBS = all)
        # os.environ['PATH'] = "%s:%s" % (bin_path, os.environ['PATH'])
        env.PrependENVPath('PATH', bin_path)
        return env
    
     # ps3toolchain with psl1ght dev environment on linux
    def ps3(env):
        print "Environment is ps3 (ps3 development)"
        # symlink the ps3dev to /opt/ps3dev, or just put it there
        # Needs these environment variables
        # export PS3DEV=/opt/ps3dev
        # export PATH=$PATH:$PS3DEV/bin
        # export PATH=$PATH:$PS3DEV/ppu/bin
        # export PATH=$PATH:$PS3DEV/spu/bin
        # export PSL1GHT=$PS3DEV/psl1ght
        # export PATH=$PATH:$PSL1GHT/bin
        path = ps3devPath()
        bin_path = path + '/ppu/bin/'
        prefix = 'ppu-'
        def setup(pre, x):
            return '%s%s' % (pre, x)
        env['CC'] = setup(prefix, 'gcc')
        env['LD'] = setup(prefix, 'ld')
        env['CXX'] = setup(prefix, 'g++')
        env['AS'] = setup(prefix, 'as')
        env['AR'] = setup(prefix, 'ar')
        env['OBJCOPY'] = setup(prefix, 'objcopy')
        # FIXME: try to use sdl-config to find these paths
        # instead of hard coding them
        scons.utils.safeParseConfig(env, path + '/portlibs/ppu/bin/freetype-config --cflags --libs')
        scons.utils.safeParseConfig(env, path + '/portlibs/ppu/bin/libpng-config --cflags --libs')

        # FIXME: it uses -lc-glue-ppu which I can't find maybe I missed something in the setup for now I'll put it down below
        #env.ParseConfig(bin_path +'sdl-config --cflags --libs') 
        
        env.Append(CPPPATH = [setup(path, "/ppu/include"),
                              setup(path, "/spu/include"),
                              setup(path, "/portlibs/ppu/include"),
                              setup(path, "/portlibs/ppu/include/SDL"),
                              setup(path, "/portlibs/ppu/include/vorbis"),
                              setup(path, "/portlibs/ppu/include/ogg"),
                              setup(path, "/psl1ght/ppu/include"),
                              setup(path, "/psl1ght/spu/include")])
        env.Append(CPPDEFINES = ['PS3'])
        env.Append(LIBPATH = [setup(path, '/ppu/lib'),
                              setup(path, '/spu/lib'),
                              setup(path, '/portlibs/ppu/lib'),
                              setup(path, '/psl1ght/ppu/lib'),
                              setup(path, '/psl1ght/spu/lib')])
        # -mminimal-toc puts less stuff in the table of contents "TOC".
        # I don't know why this is important for the ps3 compiler but apparently
        # paintown was overflowing the TOC causing it to crash on startup.
        flags = ['-mcpu=cell', '-mhard-float', '-fmodulo-sched', '-ffunction-sections', '-fdata-sections', '-maltivec', '-mminimal-toc']
        env.Append(CCFLAGS = flags)
        env.Append(CXXFLAGS = flags)
        env['LINKCOM'] = '$CC $LINKFLAGS $SOURCES -Wl,--start-group $ARCHIVES $_LIBDIRFLAGS $_LIBFLAGS -Wl,--end-group -o $TARGET'
        env.Append(LINKFLAGS = flags)
        # Removed reality and psl1ght
        all = Split("""
SDL
ogg
vorbis
vorbisfile
stdc++
m
gcm_sys
sysutil
lv2
io
net
sysmodule
audio
png
z
jpeg
c
rsx
""")
        env.Append(LIBS = all)
        env.PrependENVPath('PATH', bin_path)
        env.PrependENVPath('PKG_CONFIG_PATH', path + '/portlibs/ppu/lib/pkgconfig')
        return env
        
    # use the devkitpro stuff for wii/gamecube
    def wii(env):
        bin_path = "%s/bin" % os.environ['DEVKITPPC']
        ogc_bin_path = "%s/libogc/bin" % os.environ['DEVKITPRO']
        prefix = 'powerpc-eabi-'
        def setup(x):
            return '%s%s' % (prefix, x)
        env['CC'] = setup('gcc')
        env['LD'] = setup('ld')
        env['CXX'] = setup('g++')
        env['AS'] = setup('as')
        env['AR'] = setup('ar')
        env['OBJCOPY'] = setup('objcopy')
        if isWindows():
            env.Append(CPPDEFINES = ['USE_SDL_MAIN'])
            env.Append(CPPPATH = ["%s/libogc/include" % os.environ['DEVKITPRO'],
                "%s/libogc/include/SDL" % os.environ['DEVKITPRO'],
                "%s/libogc/include/freetype2" % os.environ['DEVKITPRO']])
            env.Append(LIBPATH = ["%s/libogc/lib" % os.environ['DEVKITPRO'],
                "%s/libogc/lib/wii" % os.environ['DEVKITPRO']])
            env.Append(LIBS = ['SDL', 'SDL_image', 'SDL_mixer', 'png', 'freetype', 'z'])
        else:
            env.Append(CPPPATH = ["%s/libogc/include" % os.environ['DEVKITPRO']])
        env.Append(CPPDEFINES = ['GEKKO', 'WII'])
        flags = ['-mrvl', '-mcpu=750', '-meabi', '-mhard-float']
        env.Append(CCFLAGS = flags)
        env.Append(CXXFLAGS = flags)
        env.Append(LINKFLAGS = flags)
        env.Append(CPPPATH = ['#src/wii'])
        env['LINKCOM'] = '$CXX $LINKFLAGS -Wl,--start-group $ARCHIVES $SOURCES $_LIBDIRFLAGS $_LIBFLAGS -Wl,--end-group -o $TARGET'
        env.Append(LIBS = ['wiiuse', 'wiikeyboard', 'iberty', 'bte', 'fat', 'ogc', 'm'])
        # os.environ['PATH'] = "%s:%s:%s" % (bin_path, ogc_bin_path, os.environ['PATH'])
        env.PrependENVPath('PATH', bin_path)
        env.PrependENVPath('PATH', ogc_bin_path)
        return env

    def androidX86(env):
        # Sets up the environment for Google Android
        def setup(pre, x):
            return '%s%s' % (pre, x)
        
        platform = 'android-9'
        arch = 'x86'
        path = '/opt/android/android-toolchain-x86'
        # bin_path = setup(path, '/arm-linux-androideabi-4.4.3/bin')
        bin_path = setup(path, '/bin')
        prefix = 'i686-android-linux-'
        def set_prefix(x):
            return '%s%s' % (prefix, x)
        env['CC'] = set_prefix('gcc')
        env['LD'] = set_prefix('ld')
        env['CXX'] = set_prefix('g++')
        env['AS'] = set_prefix('as')
        env['AR'] = set_prefix('ar')
        env['OBJCOPY'] = set_prefix('objcopy')

        base = setup(path, '/user/%(arch)s' % {'arch': arch})

        env.Append(CPPPATH = ['%s/include' % base,
                              # '%s/include/allegro5' % base
                              ])
        
        #env.Append(CPPPATH = [setup(path, '/arm-linux-androideabi-4.4.3/include'), 
        #                      setup(path, '/platforms/%s/arch-arm/usr/include' % platform),
        #                      setup(path, '/platforms/%s/arch-arm/usr/include/SDL' % platform),
        #                      setup(path, '/platforms/%s/arch-arm/usr/include/freetype' % platform),
        #                      setup(path, '/sources/cxx-stl/gnu-libstdc++/include')
        #                     ])
        env.Append(CPPDEFINES = Split("""ANDROID"""))
        # flags = ['-fpic', '-fexceptions', '-ffunction-sections', '-funwind-tables', '-fstack-protector',  '-Wno-psabi', '-march=armv5te', '-mtune=xscale', '-msoft-float', '-mthumb', '-Os', '-fomit-frame-pointer', '-fno-strict-aliasing', '-finline-limit=64',]
        flags = ['-shared', '-fpic', '-fexceptions', '-ffunction-sections', '-funwind-tables', '-Wno-psabi', '-O2', '-fno-strict-aliasing']
        # linkflags = flags + ['-Wl,--allow-shlib-undefined']
        linkflags = flags + ['-Wl,--no-undefined']
        # libs = ['freetype', 'png', 'SDL', 'm', 'log', 'jnigraphics', 'c', 'm', 'supc++',]
        # Copy the static stdc++ from gnu-libstdc++
        # gnustdlib = env.InstallAs('misc/libgnustdc++.a', '/opt/android/sources/cxx-stl/gnu-libstdc++/libs/armeabi/libstdc++.a')
        # libs = Split("""freetype2-static png SDL m log c jnigraphics supc++ EGL GLESv2 GLESv1_CM z gnustdc++""")
        # libs = Split("""freetype2-static allegro m log c jnigraphics EGL GLESv2 GLESv1_CM z gnustl_static""")
        libs = Split("""freetype2-static allegro log z""")
        env.Append(CCFLAGS = flags)
        env.Append(CXXFLAGS = flags)
        env.Append(LINKFLAGS = linkflags)
        env.Append(CPPPATH = ['#src/android'])
        env['LINKCOM'] = '$CXX $LINKFLAGS -Wl,--start-group $SOURCES $ARCHIVES $_LIBDIRFLAGS $_LIBFLAGS -Wl,--end-group -o $TARGET'
        # Hack to put libstdc++ at the end
        # env['LINKCOM'] = '$CXX $LINKFLAGS $SOURCES $_LIBDIRFLAGS $_LIBFLAGS /opt/android/sources/cxx-stl/gnu-libstdc++/libs/armeabi/libstdc++.a -o $TARGET'
        # env['LINKCOM'] = '$CXX $LINKFLAGS $SOURCES $_LIBDIRFLAGS $_LIBFLAGS -o $TARGET'
        env.Append(LIBS = libs)
        env.Append(LIBPATH = ['%s/lib' % base,
             #setup(path, '/platforms/%s/arch-arm/usr/lib' % platform),
                              ])
        
        env.PrependENVPath('PATH', bin_path)
        return env

    def android(env):
        # Sets up the environment for Google Android
        def setup(pre, x):
            return '%s%s' % (pre, x)
        
        platform = 'android-9'
        arch = 'armeabi-v7a'
        path = '/opt/android/android-toolchain'
        # bin_path = setup(path, '/arm-linux-androideabi-4.4.3/bin')
        bin_path = setup(path, '/bin')
        prefix = 'arm-linux-androideabi-'
        def set_prefix(x):
            return '%s%s' % (prefix, x)
        env['CC'] = set_prefix('gcc')
        env['LD'] = set_prefix('ld')
        env['CXX'] = set_prefix('g++')
        env['AS'] = set_prefix('as')
        env['AR'] = set_prefix('ar')
        env['OBJCOPY'] = set_prefix('objcopy')

        base = setup(path, '/user/%(arch)s' % {'arch': arch})
        
        env.PrependENVPath('PKG_CONFIG_PATH', base + '/lib/pkgconfig')

        env.Append(CPPPATH = ['%s/include' % base,
                              # '%s/include/allegro5' % base
                              ])
        
        #env.Append(CPPPATH = [setup(path, '/arm-linux-androideabi-4.4.3/include'), 
        #                      setup(path, '/platforms/%s/arch-arm/usr/include' % platform),
        #                      setup(path, '/platforms/%s/arch-arm/usr/include/SDL' % platform),
        #                      setup(path, '/platforms/%s/arch-arm/usr/include/freetype' % platform),
        #                      setup(path, '/sources/cxx-stl/gnu-libstdc++/include')
        #                     ])
        env.Append(CPPDEFINES = Split("""ANDROID __ARM_ARCH_5__ __ARM_ARCH_5T__ __ARM_ARCH_5E__ __ARM_ARCH_5TE__"""))
        # flags = ['-fpic', '-fexceptions', '-ffunction-sections', '-funwind-tables', '-fstack-protector',  '-Wno-psabi', '-march=armv5te', '-mtune=xscale', '-msoft-float', '-mthumb', '-Os', '-fomit-frame-pointer', '-fno-strict-aliasing', '-finline-limit=64',]
        flags = ['-shared', '-fpic', '-fexceptions', '-ffunction-sections', '-funwind-tables', '-Wno-psabi', '-march=armv5te', '-mtune=xscale', '-msoft-float', '-mthumb', '-Os', '-fomit-frame-pointer', '-fno-strict-aliasing', '-finline-limit=64']
        # linkflags = flags + ['-Wl,--allow-shlib-undefined']
        linkflags = flags + ['-Wl,--no-undefined']
        # libs = ['freetype', 'png', 'SDL', 'm', 'log', 'jnigraphics', 'c', 'm', 'supc++',]
        # Copy the static stdc++ from gnu-libstdc++
        # gnustdlib = env.InstallAs('misc/libgnustdc++.a', '/opt/android/sources/cxx-stl/gnu-libstdc++/libs/armeabi/libstdc++.a')
        # libs = Split("""freetype2-static png SDL m log c jnigraphics supc++ EGL GLESv2 GLESv1_CM z gnustdc++""")
        libs = Split("""freetype2-static png m log c jnigraphics EGL GLESv2 GLESv1_CM z gnustl_static""")
        env.Append(CCFLAGS = flags)
        env.Append(CXXFLAGS = flags)
        env.Append(LINKFLAGS = linkflags)
        env.Append(CPPPATH = ['#src/android'])
        env['LINKCOM'] = '$CXX $LINKFLAGS -Wl,--start-group $SOURCES $ARCHIVES $_LIBDIRFLAGS $_LIBFLAGS -Wl,--end-group -o $TARGET'
        # Hack to put libstdc++ at the end
        # env['LINKCOM'] = '$CXX $LINKFLAGS $SOURCES $_LIBDIRFLAGS $_LIBFLAGS /opt/android/sources/cxx-stl/gnu-libstdc++/libs/armeabi/libstdc++.a -o $TARGET'
        # env['LINKCOM'] = '$CXX $LINKFLAGS $SOURCES $_LIBDIRFLAGS $_LIBFLAGS -o $TARGET'
        env.Append(LIBS = libs)
        env.Append(LIBPATH = ['%s/lib' % base,
             #setup(path, '/platforms/%s/arch-arm/usr/lib' % platform),
                              ])
        
        env.PrependENVPath('PATH', bin_path)
        return env
    def ios(env):
        # Sets up the environment for Apple IOS
        # FIXME Target correct arm or simulator
        # iPhoneOS.platform
        # iPhoneSimulator.platform

        def setup(pre, x):
            return '%s%s' % (pre, x)
        
        # Check for or target above mentioned platforms
        platform = 'iPhoneOS.platform'
        # Target sdk
        sdk = 'SDKs/iPhoneOS4.3.sdk'
        path = '/Developer/Platforms/%s/Developer' % platform
        bin_path = setup(path, 'usr/bin')
        # Prefix may depend on target platform
        prefix = 'arm-apple-darwin10-'
        def set_prefix(x):
            return '%s%s' % (prefix, x)
        env['CC'] = set_prefix('gcc')
        env['LD'] = set_prefix('ld')
        env['CXX'] = set_prefix('g++')
        env['AS'] = set_prefix('as')
        env['AR'] = set_prefix('ar')
        env['OBJCOPY'] = set_prefix('objcopy')
        
        env.Append(CPPPATH = [setup(path, '/include'), 
                              setup(path, '/%s/usr/include' % sdk)
                             ])
        env.Append(CPPDEFINES = ['IOS'])
        
        flags = ['-shared', '-fpic', '-fexceptions', '-ffunction-sections', '-funwind-tables', '-Wno-psabi', '-march=armv5te', '-mtune=xscale', '-msoft-float', '-mthumb', '-Os', '-fomit-frame-pointer', '-fno-strict-aliasing', '-finline-limit=64']
        
        linkflags = flags
        
        libs = Split("""m c z""")
        env.Append(CCFLAGS = flags)
        env.Append(CXXFLAGS = flags)
        env.Append(LINKFLAGS = linkflags)
        env.Append(CPPPATH = ['#src/ios'])
        env['LINKCOM'] = '$CXX $LINKFLAGS $SOURCES -Wl,--start-group $ARCHIVES $_LIBDIRFLAGS $_LIBFLAGS -Wl,--end-group -o $TARGET'
        
        env.Append(LIBS = libs)
        env.Append(LIBPATH = [setup(path, '%s/usr/lib' % sdk),
                              '#misc'])
        
        env.PrependENVPath('PATH', bin_path)
        return env
    def nacl(env):
        # Sets up the environment for Googles Native Client
        # check for architecture
        print "Environment is for Google Native Client"
        import platform
        arch = platform.architecture()[0]
        def nacl32():
            try:
                return os.environ['nacl_32'] == '1'
            except KeyError:
                return False

        def nacl64():
            try:
                return os.environ['nacl_64'] == '1'
            except KeyError:
                return False

        arch_override = ''
        if nacl32():
            arch_override = '32bit'
            print "Forcing 32bit compile"
        elif nacl64():
            print "Forcing 64bit compile"
            arch_override = '64bit'
        
        def setup(pre, x):
            return '%s%s' % (pre, x)
        
        path = '/opt/nacl_sdk'
        # Choices are 'glibc' and 'newlib'
        toolchain_path = '/toolchain/linux_x86_newlib'
        bin_path = setup(path, '%s/bin' % toolchain_path)
        env.PrependENVPath('PATH', bin_path)
        flags = []
        paths = []
        
        libs = ['srpc', 'ppapi_cpp', 'ppapi', 'ppruntime', 'nosys']

        env['PAINTOWN_NACL_ARCH'] = '32'
        
        if arch == '64bit' and not arch_override == '32bit' or arch_override == '64bit':
            prefix = 'x86_64-nacl-'
            flags += ['-m64']
            libs.append('')
            paths.append([setup(path, '%s/x86_64-nacl/usr/include/' % toolchain_path)])
            usr_path = setup(path, '%s/x86_64-nacl/usr' % toolchain_path)
            env['PAINTOWN_NACL_ARCH'] = '64'
        elif arch == '32bit' or arch_override == '32bit':
            prefix = 'i686-nacl-'
            flags += ['-m32']
            libs.append('')
            paths.append([setup(path, '%s/i686-nacl/usr/include' % toolchain_path)])
            usr_path = setup(path, '%s/i686-nacl/usr' % toolchain_path)
        
        env.PrependENVPath('PATH', usr_path)
        env.PrependENVPath('PKG_CONFIG_PATH', "%s/%s" % (usr_path, "lib/pkgconfig"))
        paths.append([setup(usr_path,'/include')])
        
        def set_prefix(x):
            return '%s%s' % (prefix, x)
        env['CC'] = set_prefix('gcc')
        env['LD'] = set_prefix('ld')
        env['CXX'] = set_prefix('g++')
        env['AS'] = set_prefix('as')
        env['AR'] = set_prefix('ar')
        env['OBJCOPY'] = set_prefix('objcopy')
        
        scons.utils.safeParseConfig(env, usr_path + '/bin/libpng-config --cflags --libs')
        scons.utils.safeParseConfig(env, usr_path + '/bin/freetype-config --cflags --libs')
        scons.utils.safeParseConfig(env, usr_path + '/bin/sdl-config --cflags --libs')

        compile_flags = ['-fno-builtin', '-fno-stack-protector', '-fdiagnostics-show-option']

        env.Append(CPPDEFINES = ['NACL'])
        env.Append(CPPPATH = paths)
        env.Append(CCFLAGS = flags + compile_flags)
        env.Append(CXXFLAGS = flags + compile_flags)
        env.Append(LIBPATH = setup(path, '%s/lib' % toolchain_path))
        env.Append(LINKFLAGS = flags + ['-melf_nacl'])
        env['LINKCOM'] = '$CXX $LINKFLAGS $SOURCES -Wl,--start-group $ARCHIVES $_LIBDIRFLAGS $_LIBFLAGS -Wl,--end-group -o $TARGET'
        env.Append(LIBS = libs)
        wrapSymbols(env)
        return env
    def gcw(env):
        root = '/opt/gcw0-toolchain'
        env.PrependENVPath('PATH', '%s/usr/mipsel-gcw0-linux-uclibc/sysroot/usr/bin' % root)
        env.PrependENVPath('PKG_CONFIG_PATH', '%s/usr/mipsel-gcw0-linux-uclibc/sysroot/usr/lib/pkgconfig' % root)
        env.Append(CPPPATH = '%s/usr/mipsel-gcw0-linux-uclibc/sysroot/usr/include' % root)
        env.PrependENVPath('PATH', '%s/usr/bin' % root)
        env.Append(CPPDEFINES = ['UCLIBC', 'GCW0', '_FILE_OFFSET_BITS=64'])
        env['LINKCOM'] = '$CXX $LINKFLAGS $SOURCES -Wl,--start-group $ARCHIVES $_LIBDIRFLAGS $_LIBFLAGS -Wl,--end-group -o $TARGET'
        def setup(x):
            return 'mipsel-linux-%s' % x
        env['CC'] = setup('gcc')
        env['CXX'] = setup('g++')
        env['AR'] = setup('ar')
        env['LD'] = setup('ld')
        return env
    def gcc(env):
        # Allow environment variables to overwrite the default compiler
        try:
            env['CC'] = os.environ['CC']
        except:
            pass
        try:
            env['CXX'] = os.environ['CXX']
        except:
            pass

        if useDistcc():
            env['CC'] = 'distcc'
            env['CXX'] = 'distcc g++'
        if isOSX():
            env.Append(CCFLAGS = Split("""-arch i386 -arch x86_64"""))
            # env.Append(LINKFLAGS = Split("""-arch i386 -arch x86_64"""))
        if isOSX104():
            env['LINKCOM'] = '$CXX $LINKFLAGS $SOURCES $_FRAMEWORKS -Wl,-all_load $_LIBDIRFLAGS $_LIBFLAGS $ARCHIVES -o $TARGET'
        else:
            env['LINKCOM'] = '$CXX $LINKFLAGS $SOURCES -Wl,--start-group $ARCHIVES -Wl,--end-group $_LIBDIRFLAGS $_LIBFLAGS -o $TARGET'
        return env
    def llvm(env):
        #env['CC'] = 'llvm-gcc'
        #env['CXX'] = 'llvm-g++'
        env['CXX'] = 'clang++'
        env['CC'] = 'clang'
        env['LINKCOM'] = '$CXX $LINKFLAGS $SOURCES -Wl,--start-group $ARCHIVES -Wl,--end-group $_LIBDIRFLAGS $_LIBFLAGS -o $TARGET'

        if getDebug() and False:
            sanitize_flags = ['-fsanitize=undefined', '-fsanitize=integer']
            env.Append(CCFLAGS = sanitize_flags)
            env.Append(CXXFLAGS = sanitize_flags)
            env.Append(LINKFLAGS = sanitize_flags)

        # Speeds up compiles by not shelling out to 'as', but not mature yet
        # env.Append(CCFLAGS = ['-integrated-as'])

        # <nicholas> ah. don't replace AR or LD or RANLIB.
        # <jonrafkind> dont use llvm-ar and llvm-ld?
        # <nicholas> that's correct.
        # <jonrafkind> ok.. but why do those exist?
        # <nicholas> if you switch CXX to llvm-g++ and change nothing else, it'll work.
        # <jonrafkind> ok
        # <nicholas> well, that's a great question. :)
        # <jonrafkind> heh
        # <nicholas> the llvm-foo tools do what the native tools except to llvm bitcode instead
        #env['AR'] = 'llvm-ar'
        #env['AS'] = 'llvm-as'
        #env['LD'] = 'llvm-ld'
        #env['RANLIB'] = 'llvm-ranlib'
        return env
    def raw():
        defines = []
        if isOSX() or isOSX104():
            defines.append('MACOSX')
            # env.Append(CPPDEFINES = 'MACOSX')
        cflags = []
        if debug:
            defines.append('DEBUG')
            # for gcov:
            # ['-fprofile-arcs', '-ftest-coverage']
            # cflags.append( ['-g3','-ggdb', '-Werror'])
            cflags.extend(['-g3', '-O0', '-Wfatal-errors'])
        else:
            # -march=native
            if nativeCompile():
                cflags.extend(['-O2', '-g', '-pipe', '-march=native'])
                if not enableProfiled():
                    cflags.extend(['-fomit-frame-pointer'])
            else:
                cflags.extend(['-O2'])

        if isCygwin():
            import SCons.Tool.zip
            env = Environment(ENV = os.environ, tools = ['mingw'])
            env['CXX'] = 'C:\\MinGW\\bin\\g++.exe'
            env['CC'] = 'C:\\MinGW\\bin\\gcc.exe'
            env['AR'] = 'C:\\MinGW\\bin\\ar.exe'
            SCons.Tool.zip.generate(env)
            return env
        elif useMingw():
            return gcc(Environment(ENV = os.environ, CPPDEFINES = defines, CCFLAGS = cflags, tools = ['mingw', 'zip']))
        else:
            if useIntel():
                print "Using the intel compiler"
                return intel(Environment(ENV = os.environ, CPPDEFINES = defines, CCFLAGS = cflags))
            elif usePandora():
                return pandora(Environment(ENV = os.environ, CPPDEFINES = defines, CCFLAGS = cflags))
            elif useNDS():
                return nds(Environment(ENV = os.environ, CPPDEFINES = defines, CCFLAGS = cflags))
            elif useGCW():
                return gcw(Environment(ENV = os.environ, CPPDEFINES = defines, CCFLAGS = cflags))
            elif useDingoo():
                return dingux(Environment(ENV = os.environ, CPPDEFINES = defines, CCFLAGS = cflags))
            elif useXenon():
                return xenon(Environment(ENV = os.environ, CPPDEFINES = defines, CCFLAGS = cflags))
            elif useWii():
                if isWindows():
                    return wii(Environment(ENV = os.environ, CPPDEFINES = defines, CCFLAGS = cflags, tools = ['mingw']))
                return wii(Environment(ENV = os.environ, CPPDEFINES = defines, CCFLAGS = cflags))
            elif useMinpspw():
                return minpspw(Environment(ENV = os.environ, CPPDEFINES = defines, CCFLAGS = cflags, tools = ['mingw']))
            elif useAndroidX86():
                return androidX86(Environment(ENV = os.environ, CPPDEFINES = defines, CCFLAGS = cflags, tools = ['mingw']))
            elif useAndroid():
                return android(Environment(ENV = os.environ, CPPDEFINES = defines, CCFLAGS = cflags, tools = ['mingw']))
            elif useIos():
                return ios(Environment(ENV = os.environ, CPPDEFINES = defines, CCFLAGS = cflags, tools = ['mingw']))
            elif usePs3():
                return ps3(Environment(ENV = os.environ, CPPDEFINES = defines, CCFLAGS = cflags, tools = ['mingw']))
            elif useNacl():
                return nacl(Environment(ENV = os.environ, CPPDEFINES = defines, CCFLAGS = cflags, tools = ['mingw']))
            elif useLLVM():
                return llvm(Environment(ENV = os.environ, CPPDEFINES = defines, CCFLAGS = cflags))
            else:
                return gcc(Environment(ENV = os.environ, CPPDEFINES = defines, CCFLAGS = cflags))
    def add_peg(env):
        env['PEG_MAKE'] = 'Creating peg parser $TARGET'
        return env
    if not debug and not isVerbose():
        return less_verbose(raw())
    else:
        return add_peg(raw())
        
if isWindows():
    print "Try 'scons env=mingw' if you want to use mingw's gcc instead of visual studio or borland"
    if not isCygwin():
        print "Cygwin not detected. If are you using cygwin set"
        print "export CYGWIN=1"
    else:
        print "Cygwin detected"

def peg_to_cpp(target, source, env):
    import sys
    sys.path.append("src/mugen/parser")
    sys.path.append(".")
    import peg, re, cpp_generator
    name = source[0].name
    parser = peg.make_peg_parser(re.sub('\..*', '', name))
    fout = open(target[0].path, 'w')
    fout.write(cpp_generator.generate(parser(source[0].path)))
    fout.write('\n')
    fout.close()

def peg_to_cpp_multi(target, source, env):
    import sys
    sys.path.append("src/mugen/parser")
    sys.path.append(".")
    import peg, re, os
    name = re.sub('\..*', '', source[0].name)
    parser = peg.make_peg_parser(name)
    # print "Multi cpp %s" % target
    # print "Target path is %s" % target[0].path
    fout = open(target[0].path, 'w')
    directory = os.path.dirname(target[0].path)
    # fout.write(parser(source[0].path).generate_cpp(separate = name, directory = directory))
    fout.write(parser(source[0].path).generate_cpp(separate = name, directory = directory))
    # fout.write(parser(source[0].path).generate_cpp(separate = name, main = True))
    fout.write('\n')
    fout.close()
    #print "Generated multi"
    #import time
    # time.sleep(10)

def peg_scanner(node, env, path):
    import sys
    #sys.path.append("src/mugen/parser")
    #sys.path.append('../../../src/mugen/parser')
    #sys.path.append(".")
    import peg, re, os
    # print "Node is %s" % node
    name = re.sub('\..*', '', str(os.path.basename(str(node))))
    directory = os.path.dirname(str(node))
    # print "Peg emitter source is '%s' %s" % (source[0].path, source[0].srcnode())
    # print "Peg name is %s" % name
    parser = peg.make_peg_parser(name)
    # print "x1"
    where = '/tmp/scanner.tmp'
    stuff = open(where, 'w')
    stuff.write(node.get_contents())
    stuff.close()
    final = parser(where)
    files = final.list_files(name)
    # print "Generating peg files to %s" % directory
    # final.generate_cpp(separate = name, directory = directory)
    final.generate_cpp(separate = name)
    #print "Generated.."
    #import time
    #time.sleep(15)
    return files

def peg_emitter(target, source, env):
    import sys
    #sys.path.append("src/mugen/parser")
    #sys.path.append('../../../src/mugen/parser')
    #sys.path.append(".")
    import peg, re, os
    # print "Node is %s" % node
    name = re.sub('\..*', '', str(os.path.basename(str(source[0].path))))
    directory = os.path.dirname(str(target[0].path))
    parser = peg.make_peg_parser(name)
    # Hack! just make the parser read a string directly
    where = '/tmp/scanner.tmp'
    stuff = open(where, 'w')
    stuff.write(source[0].get_contents())
    stuff.close()
    final = parser(where)
    # files = final.list_files(name, directory = directory)
    files = final.list_files(name)
    # print "Generating peg files for %s from %s to %s" % (target[0], source[0], directory)
    # print "Implicit dependancies %s" % ' '.join(files)
    # final.generate_cpp(separate = name, directory = directory)
    #print "Generated.."
    #import time
    #time.sleep(15)
    # print "Emitted %s" % files
    def isHeader(file):
        return '.h' in file
    return (target + [file for file in files if not isHeader(file)], source)
    #return (target + files, source)

# This is the architecture for the new SConstruct design
def newGetEnvironments():
    def osxEnvironments():
        # Build the environments here
        environment = False
        staticEnvironment = False
        return (environment, staticEnvironment)

    def unixEnvironments():
        environment = False
        staticEnvironment = False
        return (environment, staticEnvironment)

    if isOSX():
        return osxEnvironments()
    elif scons.utils.isLinux():
        return unixEnvironments()
    # ... do rest of platforms ...

import sys
sys.path.append('src/mugen/parser')
import peg

env = getEnvironment(getDebug())

#sys.path.append('scons')
#import unix
#env = unix.getEnvironment()
#unix.configure(env)

env['ARCHIVES'] = []
if scons.utils.useSDL():
    env['PAINTOWN_BACKEND'] = 'sdl'
elif scons.utils.useAllegro():
    env['PAINTOWN_BACKEND'] = 'allegro'
elif scons.utils.useAllegro5():
    env['PAINTOWN_BACKEND'] = 'allegro5'
else:
    env['PAINTOWN_BACKEND'] = 'unknown'

# Generates a single .cpp file from a .peg description
peg_builder = Builder(action = Action(peg_to_cpp, env['PEG_MAKE']),
                      suffix = '.cpp',
                      src_suffix = '.peg')

# Generates a .cpp file for each rule in the .peg description
peg_multiple_builder = Builder(action = Action(peg_to_cpp_multi, env['PEG_MAKE']),
                               suffix = '.cpp',
                               src_suffix = '.peg',
                               # source_scanner = Scanner(peg_scanner),
                               emitter = peg_emitter
                               )

env.Append(BUILDERS = {'Peg' : peg_builder})
env.Append(BUILDERS = {'PegMulti' : peg_multiple_builder})

if showTiming():
    cxxcom = env['CXXCOM']
    cccom = env['CCCOM']
    env.Replace(CXXCOM = 'misc/show-current-time %s' % cxxcom)
    env.Replace(CCCOM = 'misc/show-current-time %s' % cccom)

env['PAINTOWN_USE_PRX'] = useMinpspw() and usePrx()
if not useMinpspw() and not useNDS() and not useDingoo() and not useXenon() and not useNacl() and not useAndroid() and not useAndroidX86() and not useIos() and not useGCW():
    env['PAINTOWN_NETWORKING'] = True
    env.Append(CPPDEFINES = ['HAVE_NETWORKING'])
else:
    env['PAINTOWN_NETWORKING'] = False

def getDataPath():
    try:
        return ARGUMENTS['data']
    except KeyError:
        if useWii():
            # consistent with homebrew
            return '/apps/paintown/data'
        if useAndroid() or useAndroidX86():
            return '/sdcard/paintown/data'
        if useXenon():
            return 'uda0:/paintown/data'
        if useMinpspw():
            return 'ms0:/psp/game150/paintown/data'
        else:
            return 'data'

debug = getDebug()
dataPath = getDataPath()

# This stuff should be moved to getEnvironment()
cflags = ['-Wall', '-Wno-unused-variable', '-Wno-unused-function']
cppflags = ['-Woverloaded-virtual']
cdefines = ['DATA_PATH=\\\"%s\\\"' % dataPath]
# cppflags = [ '-Woverloaded-virtual' ]

# env.Append( CCFLAGS = '-m32' )
# env.Append( LINKFLAGS = '-m32' )

env.Append(CCFLAGS = cflags,
           CXXFLAGS = cppflags,
           CPPPATH = ["#src"],
           CPPDEFINES = cdefines)

if env['PAINTOWN_NETWORKING']:
    env.Append(CPPPATH = ['#src/util/network/hawknl'])

# deprecated
def configEnvironment(env):
    if isWindows():
        def nothing(env):
            pass
        # dumb on windows has to have USE_ALLEGRO defined
        def doAllegro(env2):
            env2.Append(CPPDEFINES = ['USE_ALLEGRO'])
        def doSDL(env2):
            env2.Append(CPPDEFINES = ['USE_SDL'])
        env['paintown_enableAllegro'] = doAllegro
        env['paintown_enableSDL'] = doSDL
        return env
    else:
        custom_tests = {"CheckAllegro" : checkAllegro,
                        "CheckAllegro5" : scons.checks.checkAllegro5(getDebug()),
                        "CheckSDL" : scons.checks.checkSDL,
                        "CheckSDLMain" : scons.checks.checkSDLMain}
        config = env.Configure(custom_tests = custom_tests)
        if scons.utils.useAllegro():
            if not config.CheckAllegro():
                Exit(1)
        if scons.utils.useAllegro5():
            if not config.CheckAllegro5():
                Exit(1)
        if scons.utils.useSDL():
            if not config.CheckSDL():
                Exit(1)
            config.CheckSDLMain()
        return config.Finish()

# allegroEnvironment = configEnvironment(getEnvironment(debug))

def buildType(env):
    properties = []
    # Problem with command line too long under windows
    if isWindows() and useMinpspw():
        properties.append('psp')
    else:
        if scons.utils.useSDL():
            properties.append('sdl')
        if useMinpspw():
            properties.append('psp')
    if useAndroidX86():
        properties.append('android-x86')
    if useAndroid():
        properties.append('android')
    if useIos():
        properties.append('ios')
    if usePs3():
        properties.append('ps3')
    if useNacl():
        properties.append('nacl%s' % env['PAINTOWN_NACL_ARCH'])
    if usePandora():
        properties.append('Pandora')
    if useNDS():
        properties.append('NDS')
    if useGCW():
        properties.append('gcw')
    if useWii():
        properties.append('wii')
    if scons.utils.useAllegro():
        properties.append('allegro')
    if useDingoo():
        properties.append('dingoo')
    if useXenon():
        properties.append('xenon')
    if scons.utils.useAllegro5():
        properties.append('allegro5')
    if getDebug():
        properties.append('debug')
    return '-'.join(properties)

import os.path
buildDir = os.path.join('build', buildType(env))
buildDirStatic = os.path.join('build/static', buildType(env))

#if getDebug():
#    env.Append(LIBS = ['gcov'])

# change this to if True if you want to profile paintown
if enableProfiled():
    env.Append(CCFLAGS = '-pg')
    env.Append(LINKFLAGS = '-pg')

custom_tests = {"CheckPython" : scons.checks.checkPython,
                "CheckRuby" : scons.checks.checkRuby,
                "CheckRTTI" : scons.checks.checkRTTI,
                "CheckAllegro" : checkAllegro,
                "CheckAllegro5" : scons.checks.checkAllegro5(getDebug()),
                "CheckPthreads" : scons.checks.checkPthreads,
                "CheckSDL" : scons.checks.checkSDL,
                "CheckSDLMain" : scons.checks.checkSDLMain,
                "CheckOgg" : scons.checks.checkNativeOgg,
                "CheckMpg123" : scons.checks.checkMpg123,
                "CheckMad" : scons.checks.checkMad}

def display_build_properties(env):
    color = 'light-green'
    properties = []
    if scons.utils.useAllegro():
        properties.append(scons.utils.colorize("Allegro", color))
    if scons.utils.useAllegro5():
        properties.append(scons.utils.colorize('Allegro5', color))
    if scons.utils.useSDL():
        properties.append(scons.utils.colorize("SDL", color))
    if getDebug():
        properties.append(scons.utils.colorize("Debug", color))
    if enableProfiled():
        properties.append(scons.utils.colorize("Profiling", color))
    if useWii():
        properties.append(scons.utils.colorize("Wii", color))
    if usePandora():
        properties.append('Pandora')
    if useNDS():
        properties.append(scons.utils.colorize("NDS", color))
    if useMinpspw():
        properties.append(scons.utils.colorize("PSP", color))
    if useXenon():
        properties.append(scons.utils.colorize("Xenon", color))
    if useAndroid():
        properties.append(scons.utils.colorize("Android", color))
    if useGCW():
        properties.append(scons.utils.colorize("GCW", color))
    if useAndroidX86():
        properties.append(scons.utils.colorize("Android X86", color))
    if useIos():
        properties.append(scons.utils.colorize("IOS", color))
    if usePs3():
        properties.append(scons.utils.colorize("PS3", color))
    if useNacl():
        properties.append(scons.utils.colorize("NACL%s" % env['PAINTOWN_NACL_ARCH'], color))
    if useLLVM():
        properties.append(scons.utils.colorize("LLVM", color))
    if useIntel():
        properties.append(scons.utils.colorize("Intel", color))
    type = ' '.join(properties)
    if not isQuiet():
        print "Build type: %s" % type

display_build_properties(env)

env['PAINTOWN_TESTS'] = custom_tests
env['PAINTOWN_COLORIZE'] = scons.utils.colorize

if isWindows():
    staticEnv = env.Clone()

    windows_tests = {"CheckWindowsFreeType" : checkWindowsFreeType}
    config = env.Configure(custom_tests = windows_tests)
    config.CheckWindowsFreeType()
    # config.CheckPython()
    #if config.HasRuby():
    #    config.CheckRuby()
    
    env = config.Finish()

    # Removed regex.dll
    if scons.utils.useAllegro():
        env.Append(CPPDEFINES = ['USE_ALLEGRO'])
        staticEnv.Append(CPPDEFINES = ['USE_ALLEGRO'])
        env.Append( LIBS = ['alleg', 'pthread', 'png', 'freetype', 'z', 'wsock32', ] )
    elif scons.utils.useAllegro5():
        env.Append(CPPDEFINES = ['USE_ALLEGRO5'])
        staticEnv.Append(CPPDEFINES = ['USE_ALLEGRO5'])
        env.Append(LIBS = ['allegro-5.0.3-monolith-md', 'wsock32', 'z', 'freetype', 'png', 'psapi'])
    elif scons.utils.useSDL():
        if not useMinpspw() and not usePs3() and not useWii():
            env.Append(CPPDEFINES = ['USE_SDL'])
            # TODO: move this to a configure check
            env.Append(CPPPATH = ['c:/gcc4.5/include/SDL'])
            staticEnv.Append(CPPDEFINES = ['USE_SDL'])
            env.Append(LIBS = Split("""SDL pthread png user32 gdi32 winmm freetype z wsock32 psapi mpg123 vorbisfile vorbis ogg"""))
            env['HAVE_MP3_MPG123'] = True
            env.Append(CPPDEFINES = ['HAVE_MP3_MPG123'])
            env.Append(CPPDEFINES = ['HAVE_OGG'])
        elif useMinpspw() or usePs3() or useWii():
            env.Append(CPPDEFINES = ['USE_SDL'])
            staticEnv.Append(CPPDEFINES = ['USE_SDL'])
    
    if not useMinpspw() and not usePs3() and not useWii():
        env.Append( CPPDEFINES = 'WINDOWS' )
        env.Append(LINKFLAGS = ['-static-libstdc++', '-static-libgcc'])
        if getDebug():
            env.Append( CCFLAGS = ['-mthreads'] )
            env.Append( LINKFLAGS = ['-mthreads'] )
        else:
            env.Append( CCFLAGS = ['-mwindows','-mthreads'] )
            env.Append( LINKFLAGS = ['-mwindows','-mthreads'] )
    
    if scons.utils.useSDL() and not useMinpspw() or not usePs3() or not useWii():
        staticEnv.Append(LIBS = ['SDL', 'pthread', 'png', 'freetype', 'z', 'wsock32'] )
    elif scons.utils.useAllegro():
        staticEnv.Append(LIBS = [ 'alleg', 'pthread', 'png', 'freetype', 'z', 'wsock32'] )
    
    staticEnv.Append(CPPDEFINES = 'WINDOWS')
else:
    staticEnv = env.Clone()

    import sys
    # find the system tool path by attaching SCons/Tool to everything
    def fix(q):
        return q + "/SCons/Tool"
    if scons.utils.useGch():
        env.Tool('gch', toolpath = ['misc'] + [fix(e) for e in sys.path if os.path.isdir(e)])
        if not getDebug() and not isVerbose():
            env['GCHFROMHCOMSTR'] = "%s %s" % (scons.utils.colorize('Compiling header', 'green'), scons.utils.colorize('$SOURCE', 'cyan'))

    if isOSX104():
        # Build a universal binary
        staticEnv['CXX'] = 'misc/g++'
        staticEnv['CC'] = 'misc/gcc'
    elif scons.utils.isLinux() and not useWii() and not useMinpspw() and not usePs3() and not useNDS() and not useDingoo() and not useAndroid() and not useAndroidX86() and not useNacl() and not useXenon():
        staticEnv.Append(CPPDEFINES = 'LINUX')
        env.Append(CPPDEFINES = 'LINUX')
    
    # Always need libz
    env.Append(LIBS = ['z'])
    config = env.Configure(custom_tests = custom_tests)
    try:
        # config.env.ParseConfig( 'allegro-config --libs --cflags' )
        if scons.utils.useAllegro() and not config.CheckAllegro():
            print "You need the development files for Allegro. Visit Allegro's website at http://alleg.sf.net or use your package manager to install them."
            Exit(1)

        if scons.utils.useAllegro5() and not config.CheckAllegro5():
            print "Allegro5 not found"
            Exit(1)

        config.CheckPthreads()

        #if not useWii() and not useMinpspw():
        #    env.Append(LIBS = [ 'pthread' ])
        #    staticEnv.Append(LIBS = [ 'pthread' ])

        if scons.utils.useSDL() and not useMinpspw() and not usePs3() and not useNDS() and not useAndroid() and not useAndroidX86() and not useNacl():
            if not config.CheckSDL():
                print "Install libsdl 1.2"
                Exit(1)
            config.CheckSDLMain()
        elif useMinpspw() or usePs3() or useNDS() or useNacl():
            env.Append(CPPDEFINES = ['USE_SDL'])
            staticEnv.Append(CPPDEFINES = ['USE_SDL'])
            config.CheckSDLMain()
            #env.Append(CPPDEFINES = ['USE_SDL', 'USE_SDL_MAIN'])
            #staticEnv.Append(CPPDEFINES = ['USE_SDL', 'USE_SDL_MAIN'])

        if useAndroid() or useAndroidX86():
            config.CheckAllegro5()
        
        if not usePs3() and not useNacl() and not useAndroid() and not useAndroidX86():
            scons.utils.safeParseConfig(config.env, 'freetype-config --libs --cflags')
            scons.utils.safeParseConfig(config.env, 'libpng-config --libs --ldflags --cflags')
        
            # staticEnv.ParseConfig( 'allegro-config --static --libs --cflags' )
            scons.utils.safeParseConfig(staticEnv, 'freetype-config --cflags')
            scons.utils.safeParseConfig(staticEnv, 'libpng-config --cflags')
    except OSError:
        pass

    
    ## This is a hack. Copy the static libraries to misc and then link
    ## those in, otherwise gcc will try to pick the .so's from /usr/lib
    if True or not isOSX():
        png = staticEnv.Install('misc', readExec('libpng-config --libdir' ) + '/libpng.a')
        staticEnv.Append(LIBS = [png])
    else:
        scons.utils.safeParseConfig(staticEnv, 'freetype-config --libs')
        scons.utils.safeParseConfig(staticEnv, 'libpng-config --libs --ldflags --cflags')

    #if useSDL():
    #    sdl = staticEnv.Install('misc', readExec('sdl-config --prefix') + '/lib/libSDL.a')
    #    sdlmain = staticEnv.Install('misc', readExec('sdl-config --prefix') + '/lib/libSDLmain.a')
    #    staticEnv.Append(LIBS = [sdl, sdlmain])
    #    staticEnv.ParseConfig('sdl-config --cflags')
    #    staticEnv.Append(CPPDEFINES = ['USE_SDL'])

    staticEnv.Append(LIBS = ['z','m'])
    if True or not isOSX():
        path = readExec('freetype-config --prefix') + '/lib/libfreetype.a'
        if file_exists(path):
            freetype = staticEnv.Install('misc', path)
            staticEnv.Append(LIBS = freetype)
        else:
            scons.utils.safeParseConfig(staticEnv, 'freetype-config --libs')

    if not config.TryCompile("int main(){ return 0; }\n", ".c"):
        print "You need a C compiler such as gcc installed"
        Exit( 1 )
    if not config.TryCompile("class a{public: a(){} }; int main(){ a * x = new a(); delete x; return 0; }\n", ".cpp" ):
        print "You need a C++ compiler such as g++ installed"
        Exit( 1 )
    #if not config.CheckHeader( 'allegro.h' ):
    #    print "You need the header files for Allegro. Get it from http://alleg.sf.net"
    #    Exit( 1 )
    if not config.CheckHeader('ft2build.h'):
        print "You need freetype. Install freetype and/or X11"
        Exit( 1 )
    if not (useAndroid() or useAndroidX86()) and not config.CheckHeader('png.h'):
        print "You need libpng. Get it from http://www.libpng.org/pub/png/libpng.html"
        Exit(1)
    config.CheckRTTI()
    # config.CheckPython()
    config.CheckOgg()

    #if (useSDL() and not config.CheckMpg123()) or not useSDL():
    #    config.CheckMad()
    #    config.CheckMpg123()
    # Prefer mpg123 over mad because mpg123 can stream from the disk and
    # libmad is not so great at this
    if scons.utils.useMpg123():
        config.CheckMpg123()
    elif useMad():
        config.CheckMad()
    else:
        if not config.CheckMpg123():
            config.CheckMad()

    #if config.HasRuby():
    #    config.CheckRuby()
    
    env = config.Finish()

    static_custom_tests = {"CheckPython" : scons.checks.checkPython,
                           "CheckRuby" : scons.checks.checkStaticRuby,
                           "CheckAllegro" : checkAllegro,
                           "CheckSDL" : scons.checks.checkStaticSDL,
                           "CheckSDLMain" : scons.checks.checkSDLMain,
                           "CheckPthreads" : scons.checks.checkPthreads,
                           "CheckRTTI" : scons.checks.checkRTTI}
    staticEnv['PAINTOWN_TESTS'] = static_custom_tests
    static_config = staticEnv.Configure(custom_tests = static_custom_tests)
    if scons.utils.useAllegro():
        static_config.CheckAllegro()
    if scons.utils.useSDL() and not useMinpspw() or usePs3():
        static_config.CheckSDL()
        static_config.CheckSDLMain()

    static_config.CheckPthreads()

    # static_config.CheckPython()
    #if static_config.HasRuby():
    #    static_config.CheckRuby()
    static_config.CheckRTTI()
    staticEnv = static_config.Finish()


# if not isWindows():
#    env.Append(CCFLAGS = ['-Werror'])
# staticEnv.Append(CCFLAGS = ['-Werror'])

# wrapSymbols(env)
# wrapSymbols(staticEnv)

env['PAINTOWN_BUILD_TESTS'] = True
use = env
shared = SConscript('src/SConscript', variant_dir = buildDir, exports = ['use'] );

staticEnv['PAINTOWN_BUILD_TESTS'] = False
use = staticEnv
static = SConscript('src/SConscript', variant_dir = buildDirStatic, exports = ['use'] )

scripts = Split("""
src/paintown-engine/script/modules/paintown.py
""");
# Don't care about ruby for now
# src/paintown-engine/script/modules/paintown.rb

Default(Install('data/scripts', scripts))

def psp_eboot(target, source, env):
    print "Make eboot from %s to %s" % (source[0].name, target[0].name)
    file = source[0].name
    env.Execute("mksfo 'Paintown' PARAM.SFO")
    env.Execute('psp-fixup-imports %s' % file)
    if usePrx():
        prx = '%s.prx' % file
        env.Execute('psp-prxgen %s %s' % (file, prx))
        env.Execute("pack-pbp EBOOT.PBP PARAM.SFO NULL NULL NULL NULL NULL %s NULL" % prx)
    else:
        env.Execute('psp-strip %s' % file)
        env.Execute("pack-pbp EBOOT.PBP PARAM.SFO data/psp/icon0.png NULL NULL data/psp/pic1.png NULL %s NULL" % file)
    return 0

def ps3_pkg(target, source, env):
    file = source[0].name
    app = 'UP0001-Paintown_00-0000000000000000'
    print "Creating PKG for %s" % file
    if getDebug():
        if env.Execute(Copy('%s.elf' % file, file)):
            print("Could not copy %s to %s.elf" % (file, file))
            return
    else:
        env.Execute('ppu-strip %s -o %s.elf' % (file, file))
    env.Execute('%s/bin/sprxlinker %s.elf' % (ps3devPath(), file))
    env.Execute('python %s/bin/fself.py %s.elf %s.fself' % (ps3devPath(), file, file))
    env.Execute('%s/bin/make_self %s.elf %s.self' % (ps3devPath(), file, file))
    env.Execute('mkdir -p pkg/USRDIR')
    env.Execute('%s/bin/make_self_npdrm %s.elf pkg/USRDIR/EBOOT.BIN %s' % (ps3devPath(), file, app))
    # env.Execute('cp %s %s.elf' % (file, file))
    env.Execute('cp data/ps3/icon0.png pkg/ICON0.PNG')
    env.Execute('cp data/ps3/SND0.AT3 pkg/')
    env.Execute('cp data/ps3/pic1.png pkg/PIC1.PNG')
    # env.Execute('python /opt/ps3dev/bin/fself.py -n %s.elf pkg/USRDIR/EBOOT.BIN' % file)
    env.Execute('python %s/bin/sfo.py --title "Paintown" --appid "PAINTOWN" -f %s/bin/sfo.xml pkg/PARAM.SFO' % (ps3devPath(), ps3devPath()))
    env.Execute('python %s/bin/pkg.py --contentid %s pkg/ %s.pkg' % (ps3devPath(), app, file))
    env.Execute('python %s/bin/pkg.py --contentid %s pkg/ %s.retail.pkg' % (ps3devPath(), app, file))
    env.Execute('%s/bin/package_finalize %s.retail.pkg' % (ps3devPath(), file));
    print "Sign pkg with tools from geohot or something (http://www.geohot.com)..."
    return 0

def xenon_elf(target, source, env):
    file = source[0].name
    print "Running xenon-objcopy for you"
    env.Execute('xenon-objcopy -O elf32-powerpc --adjust-vma 0x80000000 %s xenon.elf' % file)
    return 0

def wii_elf2dol(target, source, env):
    file = source[0].name
    print "Running elf2dol to create %s.dol for you" % file
    env.Execute('elf2dol %s boot.dol' % (file))
    print "Rename %s to %s.elf if you want to run in dolphin with debugging" % (file, file)
    return 0

def wii_show_data(target, source, env):
    print "Wii data path is %s" % scons.utils.colorize(getDataPath(), 'light-green')
    return 0

for i in shared:
    safe = env.Install('.', i)
    if useMinpspw():
        env.AddPostAction(safe, psp_eboot)
    if useXenon():
        env.AddPostAction(safe, xenon_elf)
    if usePs3():
        env.AddPostAction(safe, ps3_pkg)
    if useWii():
        env.AddPostAction(safe, wii_elf2dol)
        env.AddPostAction(safe, wii_show_data)
    Default(safe)

for i in static:
    installed = staticEnv.InstallAs(i[0].name + '-static', i)
    Alias('static', installed)

#if isQuiet():
#    Progress(['-\r', '\\\r', '|\r', '/\r'], interval=5)
