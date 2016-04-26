#!/usr/bin/Rscript

library(methods)

optimize_PopED <- function(filename) {

  if (Sys.info()['sysname'] == "Windows") {
    command = "..\\main.exe"
  } else {
    command = "../main"
  }

  system(paste0(command, " ", filename))
}



filename = "../Executable_Simeoni_2004_oncology_TGI.xml"
optimize_PopED(filename)