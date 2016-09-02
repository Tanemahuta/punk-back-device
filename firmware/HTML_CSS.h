#ifndef HTML_CSS_H
#define HTML_CSS_H

#define PATH_CSS "/app.css"

/** HTML */
char* HTML_HEAD = "<html><head><title>F-Device configuration</title>"
                  "<link rel=\"stylesheet\" href=\""
                  PATH_CSS
                  "\">"
                  "</head>";
char* HTML_BODY = "<body>";
char* BODY_TITLE_START = "<h1>";
char* BODY_TITLE_END =   "</h1>";

/** CSS */
char* CSS_CONTENT =  "body { background-color: #223333; font-family: 'ArialMT', Arial; align:center; text-align:center; color: white; }\n"
                     "h1, h2 { text-align:center; }\n"
                     "a { color: #eeffff; text-decoration:none; }\n"
                     "a:hover { color: #fffff; text-decoration:none; border-bottom: 1px dotted; }\n"
                     ".message { color: #ff4444; text-align:center; padding: 20px; }\n"
                     ".linkCollection { min-width:300px; align:left; text-align:left; }\n"
                     "input { background-color: #445555; border: 1px solid #334444; color: #eeffff; border-radius: 5px; }\n"
                     "input:hover { background-color: #556666; border: 1px solid #445555; }\n"
                     "label, input[type=\"text\"], input[type=\"password\"] { min-width: 250px; text-align:left; align: left; margin: 5px; padding: 4px; }";




#endif
