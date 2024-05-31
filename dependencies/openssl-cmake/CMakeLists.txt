message(STATUS "Fetching OpenSSL...")

# Основана на openssl от janbar и не требует Perl, Python и др.

FetchContent_MakeAvailable(openssl-cmake)