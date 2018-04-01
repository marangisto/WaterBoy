#pragma once

const char *main_form_preamble = "<!DOCTYPE html>\n"
    "<html>\n"
    "<title>WaterGate</title>\n"
    "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n"
    "<link rel=\"stylesheet\" href=\"https://www.w3schools.com/w3css/4/w3.css\">\n"
    "<link rel=\"stylesheet\" href=\"https://www.w3schools.com/w3css/4/w3mobile.css\">\n"
    "<style>.footer{ position: fixed; left: 0; bottom: 0; width: 100%; }</style>\n"
    "<body>\n"
        "<header class=\"w3-container w3-teal\">\n"
            "<h1>WaterGate</h1>\n"
        "</header>\n"
        "<hr>\n"
        "<div class=\"w3-cell-row\">\n"
            "<div class=\"w3-cell\">\n"
  	            "<div class=\"w3-center\">\n"
  	                "<div class=\"bar\">\n"
;

const String main_form_button(int i, bool state)
{
    String n(i);
    String c(state ? "green" : "gray");

    return String("<a href=\"/") + n + String("\" class=\"w3-button w3-") + c + String(" w3-round-large\">Led ") + n + String("</a>\n");
}

const char *main_form_postamble = "<!DOCTYPE html>\n"
                    "</div>\n"
                    "<br>\n"
  	                "<div class=\"bar\">\n"
    	                "<a href=\"/clr\" class=\"w3-button w3-red w3-round-large\">Clear</a>\n"
                    "</div>\n"
                "</div>\n"
            "</div>\n"
        "</div>  \n"
        "<hr>\n"

        "<footer class=\"footer w3-teal\">\n"
        "<div class=\"w3-bar\">\n"
            "<span class=\"w3-bar-item\">&copy Marangisto 2018</span>\n"
            "<span class=\"w3-bar-item w3-right\">Version 1.0</span>\n"
        "</div>\n"
        "</footer>\n"
    "</body>\n"
;

const char *redirect_home = "<!DOCTYPE html>\n"
    "<html>\n"
        "<meta http-equiv=\"refresh\" content=\"0;/\">\n"
    "</html>\n"
;

