
# modified version of
# http://stackoverflow.com/questions/18968979
if (NOT WIN32)
  string(ASCII 27 Esc)
  set(ColourReset "${Esc}[0m")
  set(ColourBold  "${Esc}[1m")
  set(ColourRed         "${Esc}[31m")
  set(ColourGreen       "${Esc}[32m")
  set(ColourYellow      "${Esc}[33m")
  set(ColourBlue        "${Esc}[34m")
  set(ColourMagenta     "${Esc}[35m")
  set(ColourCyan        "${Esc}[36m")
  set(ColourWhite       "${Esc}[37m")
  set(ColourBoldRed     "${Esc}[1;31m")
  set(ColourBoldGreen   "${Esc}[1;32m")
  set(ColourBoldYellow  "${Esc}[1;33m")
  set(ColourBoldBlue    "${Esc}[1;34m")
  set(ColourBoldMagenta "${Esc}[1;35m")
  set(ColourBoldCyan    "${Esc}[1;36m")
  set(ColourBoldWhite   "${Esc}[1;37m")
endif()