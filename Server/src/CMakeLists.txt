project(left4dead)

FILE(GLOB_RECURSE myFiles *.cpp *.h)

add_library(server_classes
        ${myFiles}
        )