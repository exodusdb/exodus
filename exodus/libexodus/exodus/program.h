#ifndef EXODUS_LIBEXODUS_EXODUS_PROGRAM_H_
#define EXODUS_LIBEXODUS_EXODUS_PROGRAM_H_
#include <exodus/exodus.h>

// An Exodus command line program is just a C++ class that inherits from ExodusProgram and
// is followed by a standard free "int main()" function. int main instantiates an object
// of the exodus program class and calls its int main member function.
//
// The above functionality is provided by the programinit() and programexit() macros.
//
// Programmers must write an int main member function between the programinit/exit statements.
//
// The section of an exodus program delimited by programinit/exit statements simulates
// a normal PickOS/multivalue Basic program with global variables and local subroutines.
//
// Unlike PickOS/multivalue Basic, local subroutines can have parameters, return values and private
// variables.

// There must be one matching pair of programinit/exit statements per compilable file with empty
// argument. Additional matching pairs of statements can delineate additional subprograms as long
// as each pair has a unique argument since their argument names are used as part of the class name.

//#undef function
//#undef subroutine
//#undef subroutine_noreturn
//#define function public: var
//#define subroutine public: void
//#define subroutine_noreturn public: [[noreturn]] void

// SIMILAR CODE IN
// program.h library.h

// A program is just a class with an following
// int main() function that creates and calls it
#define programinit(PROGRAMCLASSNAME)                               \
_Pragma("GCC diagnostic push")                                      \
_Pragma("clang diagnostic ignored \"-Wweak-vtables\"")              \
_Pragma("GCC diagnostic ignored \"-Winline\"")                      \
class PROGRAMCLASSNAME##ExodusProgram : public ExodusProgramBase {  \
_Pragma("GCC diagnostic pop")

//OPTION I=Ignore. Causes error exit to be suppressed
//OPTION D=Debug. Suppress try/catch exception handling so debuggers can catch errors
#define programexit(PROGRAMCLASSNAME)                                                                           \
 public:                                                                                                        \
_Pragma("GCC diagnostic push")                                                                                  \
_Pragma("clang diagnostic ignored \"-Wshadow-field\"")                                                          \
                                                                                                                \
    PROGRAMCLASSNAME##ExodusProgram(ExoEnv& mv) : ExodusProgramBase(mv) {}                                      \
                                                                                                                \
_Pragma("GCC diagnostic pop")                                                                                   \
};                                                                                                              \
                                                                                                                \
_Pragma("GCC diagnostic push")                                                                                  \
_Pragma("clang diagnostic ignored \"-Wshadow-field\"")                                                          \
_Pragma("GCC diagnostic ignored \"-Winline\"")                                                                  \
                                                                                                                \
static int PROGRAMCLASSNAME##main2(int exodus_argc, const char* exodus_argv[], int threadno) {                  \
                                                                                                                \
        ExoEnv mv;                                                                                              \
        exodus_main(exodus_argc, exodus_argv, mv, threadno);                                                    \
                                                                                                                \
        int result = 0;                                                                                         \
        PROGRAMCLASSNAME##ExodusProgram exodusprogram1(mv);                                                     \
        if (osgetenv("EXO_DEBUG")) {                                                                            \
            errputl("Debug Init Thread:", THREADNO, #PROGRAMCLASSNAME, SENTENCE);                               \
            result = exodusprogram1.main().toInt();                                                             \
            errputl("Debug Exit Thread:", THREADNO, #PROGRAMCLASSNAME, SENTENCE);                               \
        } else {                                                                                                \
            try {                                                                                               \
                result = exodusprogram1.main().toInt();                                                         \
            } catch (const MVStop& exceptionx) {                                                                \
                if (exceptionx.description.len())                                                               \
                    exceptionx.description.outputl();                                                           \
                if (exceptionx.description.isnum())                                                             \
                    result = exceptionx.description.toInt();                                                    \
            } catch (const MVAbort& exceptionx) {                                                               \
                if (exceptionx.description.len())                                                               \
                    exceptionx.description.errputl();                                                           \
                if (exceptionx.description.isnum() && exceptionx.description)                                   \
                    result = exceptionx.description;                                                            \
                else                                                                                            \
                    result = 1;                                                                                 \
            } catch (const MVAbortAll& exceptionx) {                                                            \
                if (exceptionx.description.len())                                                               \
                    exceptionx.description.errputl();                                                           \
                if (exceptionx.description.isnum() && exceptionx.description)                                   \
                    result = exceptionx.description.toInt();                                                    \
                else                                                                                            \
                    result = 2;                                                                                 \
            } catch (const MVLogoff& exceptionx) {                                                              \
                if (exceptionx.description.len())                                                               \
                    exceptionx.description.outputl();                                                           \
                if (exceptionx.description.isnum())                                                             \
                    result = exceptionx.description.toInt();                                                    \
            }                                                                                                   \
            catch (const VarError& exceptionx)                                                                  \
            {                                                                                                   \
                errputl(exceptionx.description, " - Aborting.");                                                \
                errputl(var(exceptionx.stack()).convert(FM, "\n"));                                             \
                result = OPTIONS.contains("I") ? 0 : 999;                                                       \
            }                                                                                                   \
        }                                                                                                       \
        /*disconnect ALL connections of this thread*/                                                           \
        disconnectall();                                                                                        \
        disconnect();                                                                                           \
        if (OPTIONS.contains("I"))                                                                              \
            result = 0;                                                                                         \
        return result;                                                                                          \
    }                                                                                                           \
                                                                                                                \
_Pragma("GCC diagnostic pop")                                                                                   \
                                                                                                                \
    int PROGRAMCLASSNAME##main(int exodus_argc, const char* exodus_argv[]);                                     \
    int PROGRAMCLASSNAME##main(int exodus_argc, const char* exodus_argv[]) {                                    \
        return PROGRAMCLASSNAME##main2(exodus_argc, exodus_argv, 0);                                            \
    }

#endif // EXODUS_LIBEXODUS_EXODUS_PROGRAM_H_
