include(FetchContent)
function(add_libraries NAME PATH TAG)
    FetchContent_Declare(
            ${NAME}
            GIT_REPOSITORY ${PATH}
            GIT_TAG        ${TAG}
    )
    message(STATUS "Fetching ${NAME} from ${PATH} with tag ${TAG}")
    FetchContent_MakeAvailable(${NAME})
endfunction()


function(add_libraries_with_args NAME PATH TAG ARGS)
    FetchContent_Declare(
            ${NAME}
            GIT_REPOSITORY ${PATH}
            GIT_TAG        ${TAG}
    )
    message(STATUS "Fetching ${NAME} from ${PATH} with tag ${TAG} and args '${ARGS}'")
    set(ARGS_LISTS "")
    foreach(ARG ${ARGS})
        string(REPLACE "=" ";" ARG_LIST ${ARG})
        list(GET ARG_LIST 0 ARG_NAME)
        list(GET ARG_LIST 1 ARG_VALUE)
        set(${ARG_NAME} ${ARG_VALUE} CACHE INTERNAL "" FORCE)
        list(APPEND ARGS_LISTS ${ARG_NAME})
    endforeach ()
    FetchContent_MakeAvailable(${NAME})
    foreach(ARG ${ARGS_LISTS})
        unset(${ARG} CACHE)
    endforeach ()
endfunction()

