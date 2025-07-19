#ifndef CONFIG_HPP
#define CONFIG_HPP

#define CONFIG_VER_STR "dev 0.003"

#define CONFIG_CREDITS_STR "Sand:Box PC\n\
Release dev 0.003\n" __DATE__ "\n\
Written by maniek86\n\
Copyright 2025 (C)\n\
\n\
inspired by early\n\
versions of Android\n\
game Sand:Box by\n\
SmellyMoo\
"

#define CONFIG_WEBSITE_ADDR "https://maniek86.xyz"
#define CONFIG_CHANGELOG_ADDR "https://maniek86.xyz/projects.php"

// defining it doesnt remove embeded resources from build
// it just makes the game load files from resources/ instead using built in ones
//#define USE_EXTERNAL_RESOURCES

#endif
