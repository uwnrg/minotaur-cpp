function(log MESSAGE)
    message(STATUS ${MESSAGE})
endfunction()

function(error MESSAGE)
    message(FATAL_ERROR ${MESSAGE})
endfunction()

function(warning MESSAGE)
    message(WARNING ${MESSAGE})
endfunction()
