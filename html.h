/* heater_html*/
const char heater_html[] PROGMEM = {
"<!DOCTYPE html>"
"<html>"
"<head>"
"    <title>(Type a title for your page here)</title>"
"    <link href=\"ESP8266.css\" rel=\"stylesheet\"/>"
"    <script src=\"webduino.js\"></script>"
"    <script src=\"heater.js\"></script>"
"</head>"
""
"<body>"
"<!-- The Modal -->"
"<div id=\"modalPopup\" class=\"modal\">"
"    <!-- Modal content -->"
"    <div class=\"modal-content\">"
"        <span class=\"close\">&times;</span>"
"        <div id=\"modalText\">Some text in the Modal..</div>"
"        <input type=\"button\" id=\"okButton\" value=\"Ok\">"
"        <input type=\"button\" id=\"cancelButton\" value=\"Annulla\">"
"    </div>"
"</div>"
""
"<!--copiare da qui-->"
"<h1 id=\"head\">Webduino</h1>"
"<ul id=\"navigation\">"
"    <li><a href=\"index.html\" class=\"button hvr-underline-from-center\">Home</a></li>"
"    <li><a href=\"settings.html\" class=\"button hvr-underline-from-center\">Impostazioni</a></li>"
"    <li><span class=\"active\">Riscaldamento</span></li>"
"</ul>"
"<!--Content--->"
"<div id=\"content\" class=\"container_16 clearfix\">"
"    <div class=\"grid_5\">"
"        <div class=\"box\">"
""
"            <h2>Riscaldamento</h2>"
"            <form action='/command' method='POST' id='heater'>"
"                <p>"
"                    <strong>Abilitato : </strong><label class=\"input-toggle\"><input type=\"checkbox\" id=\"heaterEnabled\""
"                                                                                    name=\"enabled\"><span></span></label><br>"
"                    <strong>Pin rele : </strong><label id=\"pinSelectValue\" hidden>audi</label>"
"                    <select id=\"pinSelect\" name=\"pin\">"
"                        <option value=\"D1\">D1</option>"
"                        <option value=\"D2\">D2</option>"
"                        <option value=\"D3\">D3</option>"
"                        <option value=\"D4\">D4</option>"
"                        <option value=\"D5\">D5</option>"
"                    </select>"
"                    <br><br>"
"                    <input type='hidden' name='command' value='heatersettings'>"
"                    <input type='submit' value=\"save\">"
"                </p>"
"            </form>"
"        </div>"
""
"        <div class=\"box\">"
""
"            <h2>Riscaldamento</h2>"
""
"            <label id=\"summary\">summary</label><br>"
"            <label id=\"command\">command</label><br>"
"            <label id=\"timer\">timer</label>"
""
"            <form action='/command' method='POST' id='startManualForm'>"
"                <p>"
"                    Minutesy:<input type='number' name='duration' value='30' min=\"1\" max=\"30\"><BR>"
"                    Target:<input type='number' name='target' value='22.0' step='0.10'><BR>"
"                    Sensor:<input type='radio' name='sensor' value='0' checked>Local<input type='radio' name='sensor'"
"                                                                                           value='1'>Remote<BR>"
"                    <input type='hidden' name='command' value='manual'>"
"                    <input type='submit' value='start manual'>"
"                </p>"
"            </form>"
""
"            <form action='/command' method='POST' id='manualOffForm'>"
"                <p>"
"                    Minutesx:<input type='number' name='duration' value='30' min=\"1\" max=\"30\"><BR>"
"                    <input type='hidden' name='command' value='manualoff'>"
"                    <input type='submit' value='start manual off'>"
"                </p>"
"            </form>"
""
""
"            <div id='manual'>"
"                <form action='/command' method='POST' id='stopManualForm'>"
"                    <p>"
"                        <input type='hidden' name='command' value='manualend'>"
"                        <input type='submit' value='stop manual'>"
"                    </p>"
"                </form>"
"            </div>"
""
"        </div>"
""
""
"    </div>"
"</div>"
"<!--Footer --->"
"<div id=\"foot\">"
"    <div class=\"container_16 clearfix\">"
"        <div class=\"grid_16\"><a href=\"#\">Contact Me</a></div>"
"    </div>"
"</div>"
"<script type=\"text/javascript\">"
"    document.addEventListener(\"DOMContentLoaded\", function() {"
"        load();"
"    });"
"</script>"
"</body>"
"</html>"
};

/* index_html*/
const char index_html[] PROGMEM = {
"<!DOCTYPE html>"
"<html>"
"<head>"
"    <title>(Type a title for your page here)</title>"
"    <link href=\"ESP8266.css\" rel=\"stylesheet\"/>"
"    <script src=\"webduino.js\"></script>"
"    <script src=\"index.js\"></script>"
"</head>"
""
"<body>"
"<!-- The Modal -->"
"<div id=\"modalPopup\" class=\"modal\">"
"    <!-- Modal content -->"
"    <div class=\"modal-content\">"
"        <span class=\"close\">&times;</span>"
"        <div id=\"modalText\">Some text in the Modal..</div>"
"        <input type=\"button\" id=\"okButton\" value=\"Ok\">"
"        <input type=\"button\" id=\"cancelButton\" value=\"Annulla\">"
"    </div>"
"</div>"
""
"<!--copiare da qui-->"
"<h1 id=\"head\">Webduino</h1>"
"<ul id=\"navigation\">"
"    <li><span class=\"active\">Home</span></li>"
"    <li><a href=\"settings.html\" class=\"button hvr-underline-from-center\">Impostazioni</a></li>"
"    <li><a href=\"heater.html\" class=\"button hvr-underline-from-center\">Riscaldamento</a></li>"
"</ul>"
"<!--Content--->"
"<div id=\"content\" class=\"container_16 clearfix\">"
"    <div class=\"grid_5\">"
""
"        <input type=\"button\" id=\"popupButton\" value=\"popup\">"
""
"        <div class=\"box\">"
""
"            <h2>Stato scheda</h2>"
"            <p>"
"                <strong>Date e ora : </strong> 01-01-1970 00:01:09 <br>"
"                <strong>IP Address : </strong> 192.168.1.9<br>"
"                <strong>MAC Address : </strong> 18:fe:34:d4:c6:87 <br>"
"                <strong>ID scheda: </strong> 0 <br>"
""
"            </p>"
"        </div>"
""
"        <div class=\"box\">"
""
"            <h2>Comandi</h2>"
""
"            <label id=\"summary\">summary</label><br>"
"            <label id=\"command\">command</label><br>"
"            <label id=\"timer\">timer</label>"
""
"                <p>"
"                    <strong>Power : </strong><label class=\"input-toggle\"><input type=\"checkbox\" id=\"powerCheckBox\"><span></span></label><br><br>"
"                    <strong>Reset : </strong><input type=\"button\" id=\"resetButton\" value=\"reset\"><span></span></label>"
"                    <br><br>"
"                    <strong>Shield : </strong><label class=\"input-toggle\"><input type=\"button\""
"                                                                                 id=\"shieldButton\"><span></span></label><br><br>"
""
""
"                    <input type='hidden' name='command' value='manual'>"
""
"                </p>"
""
""
""
"        </div>"
""
""
"    </div>"
"</div>"
"<!--Footer --->"
"<div id=\"foot\">"
"    <div class=\"container_16 clearfix\">"
"        <div class=\"grid_16\"><a href=\"#\">Contact Me</a></div>"
"    </div>"
"</div>"
"<script type=\"text/javascript\">"
"    document.addEventListener(\"DOMContentLoaded\", function() {"
"        load();"
"    });"
"</script>"
"</body>"
"</html>"
};

/* prova_html*/
const char prova_html[] PROGMEM = {
""
""
"<!DOCTYPE html><html><head>    <title>(Type a title for your page here)</title>    <link href=\"ESP8266.css\" rel=\"stylesheet\"/>    <script src=\"webduino.js\"></script>    <script src=\"index.js\"></script></head><body><!-- The Modal --><div id=\"modalPopup\" class=\"modal\">    <!-- Modal content -->    <div class=\"modal-content\">        <span class=\"close\">&times;</span>        <div id=\"modalText\">Some text in the Modal..</div>        <input type=\"button\" id=\"okButton\" value=\"Ok\">        <input type=\"button\" id=\"cancelButton\" value=\"Annulla\">    </div></div><!--copiare da qui--><h1 id=\"head\">Webduino</h1><ul id=\"navigation\">    <li><span class=\"active\">Home</span></li>    <li><a href=\"settings.html\" class=\"button hvr-underline-from-center\">Impostazioni</a></li>    <li><a href=\"heater.html\" class=\"button hvr-underline-from-center\">Riscaldamento</a></li></ul><!--Content---><div id=\"content\" class=\"container_16 clearfix\">    <div class=\"grid_5\">        <input type=\"button\" id=\"popupButton\" value=\"popup\">        <div class=\"box\">            <h2>Stato scheda</h2>            <p>                <strong>Date e ora : </strong> 01-01-1970 00:01:09 <br>                <strong>IP Address : </strong> 192.168.1.9<br>                <strong>MAC Address : </strong> 18:fe:34:d4:c6:87 <br>                <strong>ID scheda: </strong> 0 <br>            </p>        </div>        <div class=\"box\">            <h2>Comandi</h2>            <label id=\"summary\">summary</label><br>            <label id=\"command\">command</label><br>            <label id=\"timer\">timer</label>                <p>                    <strong>Power : </strong><label class=\"input-toggle\"><input type=\"checkbox\" id=\"powerCheckBox\"><span></span></label><br><br>                    <strong>Reset : </strong><input type=\"button\" id=\"resetButton\" value=\"reset\"><span></span></label>                    <br><br>                    <strong>Shield : </strong><label class=\"input-toggle\"><input type=\"button\"                                                                                 id=\"shieldButton\"><span></span></label><br><br>                    <input type='hidden' name='command' value='manual'>                </p>        </div>    </div></div><!--Footer ---><div id=\"foot\">    <div class=\"container_16 clearfix\">        <div class=\"grid_16\"><a href=\"#\">Contact Me</a></div>    </div></div><script type=\"text/javascript\"> alert('prova');   load();</script></body></html>"
};

/* settings_html*/
const char settings_html[] PROGMEM = {
"<!DOCTYPE html>"
"<html>"
"<head>"
"    <title>(Type a title for your page here)</title>"
"    <link href=\"ESP8266.css\" rel=\"stylesheet\"/>"
"    <script src=\"webduino.js\"></script>"
"    <script src=\"settings.js\"></script>"
"</head>"
""
""
"<body>"
"<!-- The Modal -->"
"<div id=\"modalPopup\" class=\"modal\">"
"    <!-- Modal content -->"
"    <div class=\"modal-content\">"
"        <span class=\"close\">&times;</span>"
"        <div id=\"modalText\">Some text in the Modal..</div>"
"        <input type=\"button\" id=\"okButton\" value=\"Ok\">"
"        <input type=\"button\" id=\"cancelButton\" value=\"Annulla\">"
"    </div>"
"</div>"
""
"<!--copiare da qui-->"
"<h1 id=\"head\">Webduino</h1>"
"<ul id=\"navigation\">"
"    <li><a href=\"index.html\" class=\"button hvr-underline-from-center\">Home</a></li>"
"    <li><span class=\"active\">Impostazioni</span></li>"
"    <li><a href=\"heater.html\" class=\"button hvr-underline-from-center\">Riscaldamento</a></li>"
"</ul>"
"<!--Content--->"
"<div id=\"content\" class=\"container_16 clearfix\">"
"    <div class=\"grid_5\">"
"        <div class=\"box\">"
""
"            <h2>Impostazioni scheda</h2>"
""
"            <label id=\"summary\">summary</label><br>"
"            <label id=\"command\">command</label><br>"
"            <label id=\"timer\">timer</label>"
""
"            <form action='/command' method='POST' id='shield'>"
"                <p>"
"                    <strong>Local Port: </strong><input type='number' id='localport' name='localport' value='80' step='1' min='1' max='9999'><BR><BR>"
"                    <strong>Shield name: </strong><input type='text'  id='shieldname' name='shieldname' value='shieldname' size='29' maxlength='29'><BR><BR>"
"                    <strong>SSID: </strong><input type='text' id='ssid' name='ssid' value='SSID' size='32' maxlength='32'><BR><BR>"
"                    <strong>password: </strong><input type='text' id='password' name='password' value='password' size='96' maxlength='96'><BR><BR>"
"                    <strong>Server name: </strong><input type='text' id='servername' name='servername' value='Server name' size='29' maxlength='29'><BR><BR>"
"                    <strong>Server Port: </strong><input type='number' id='serverport' name='serverport' value='8080' step='1' min='1' max='9999'><BR><BR>"
""
"                    <input type='hidden' name='command' value='shieldsettings'>"
"                    <input type='submit' value=\"save\" onclick='whichPressed=this'>"
"                </p>"
"            </form>"
"        </div>"
""
"    </div>"
"</div>"
"<!--Footer --->"
"<div id=\"foot\">"
"    <div class=\"container_16 clearfix\">"
"        <div class=\"grid_16\"><a href=\"#\">Contact Me</a></div>"
"    </div>"
"</div>"
"<script type=\"text/javascript\">"
"    document.addEventListener(\"DOMContentLoaded\", function() {"
"        load();"
"    });"
"</script>"
"</body>"
"</html>"
};

/* button_css*/
const char button_css[] PROGMEM = {
".btn {"
"    background: #3498db;"
"    background-image: -webkit-linear-gradient(top, #3498db, #2980b9);"
"    background-image: -moz-linear-gradient(top, #3498db, #2980b9);"
"    background-image: -ms-linear-gradient(top, #3498db, #2980b9);"
"    background-image: -o-linear-gradient(top, #3498db, #2980b9);"
"    background-image: linear-gradient(to bottom, #3498db, #2980b9);"
"    -webkit-border-radius: 28;"
"    -moz-border-radius: 28;"
"    border-radius: 28px;"
"    font-family: Arial;"
"    color: #ffffff;"
"    font-size: 12px;"
"    background: #3498db;"
"    padding: 5px 10px 5px 10px;"
"    border: solid #1f628d 0px;"
"    text-decoration: none;"
"}"
""
".btn:hover {"
"    text-decoration: none;"
"}"
};

/* ESP8266_css*/
const char ESP8266_css[] PROGMEM = {
"/* popup window*/"
".ontop {"
"    z-index: 999;"
"    width: 100%;"
"    height: 100%;"
"    top: 0;"
"    left: 0;"
"    display: none;"
"    position: absolute;"
"    background-color: #636363;"
"    color: #aaaaaa;"
"    opacity: .4;"
"    filter: alpha(opacity = 50);"
"}"
"#popup {"
"    width: 250px;"
"    height: 100px;"
"    position: absolute;"
"    color: #000000;"
"    background-color: #ffffff;"
"    border-radius: 10px;"
"    top: 50%;"
"    left: 50%;"
""
"    margin-left: -125px;"
"    margin-top: -50px;"
"}"
""
"/* The Modal (background) */"
".modal {"
"    display: none; /* Hidden by default */"
"    position: fixed; /* Stay in place */"
"    z-index: 1; /* Sit on top */"
"    padding-top: 100px; /* Location of the box */"
"    left: 0;"
"    top: 0;"
"    width: 100%; /* Full width */"
"    height: 100%; /* Full height */"
"    overflow: auto; /* Enable scroll if needed */"
"    background-color: rgb(0,0,0); /* Fallback color */"
"    background-color: rgba(0,0,0,0.4); /* Black w/ opacity */"
"}"
""
"/* Modal Content */"
".modal-content {"
"    background-color: #fefefe;"
"    margin: auto;"
"    padding: 20px;"
"    border: 1px solid #888;"
"    width: 80%;"
"}"
""
"/* The Close Button */"
".close {"
"    color: #aaaaaa;"
"    float: right;"
"    font-size: 28px;"
"    font-weight: bold;"
"}"
""
".close:hover,"
".close:focus {"
"    color: #000;"
"    text-decoration: none;"
"    cursor: pointer;"
"}"
""
"#popupchild {"
"    position: absolute;"
"    top: 10%;"
"    left: 10%;"
"    height: 80%;"
"    width: 80%;"
""
"}"
"/*****/"
""
"#head {"
"    background: #313230;"
"    -moz-border-radius-topright: 10px;"
"    border: 0;"
"    margin: 10px auto 0 auto;"
"    padding: 5px 30px;"
"}"
"#content {"
"    padding: 20px;"
"    background: #fff url(gradient.png) repeat-x 0 -28px;"
"}"
"body {"
"    background-color: lightblue;"
"    color: #333;"
"    font: 13px/1.5 'Helvetica Neue', Arial, 'Liberation Sans', FreeSans, sans-serif;"
"    margin: 0px;"
"}"
"h1 {"
"    color: white;"
"    font-size: 25px;"
"    display: block;"
"    font-size: 2em;"
"    font-weight: bold;"
"    margin-bottom: 0px;"
"    margin-top: 0px;"
"}"
"h2 {"
"    background-image: linear-gradient("
"            white, rgb(221, 221, 221)"
"    );"
"    border-bottom: 1px solid;"
"    border-color: #ddd;"
"    margin-bottom: 0px;"
"    margin-top: 0px;"
""
"    padding: 0px 10px;"
"}"
"p {"
"    padding: 0px 10px;"
"}"
"#navigation {"
"    background: #434A48;"
"    color: #fff;"
"    padding: 10px 30px;"
"}"
"#navigation .active {"
"    background: #687370;"
"}"
"#navigation a:hover {"
"    background: #232323;"
"}"
"#navigation > li {"
"    display: inline;"
"    margin: 0;"
"}"
"#navigation > li > span {"
"    display: inline-block;"
"    padding: 5px 10px;"
"    text-decoration: none;"
"    font-family: \"Helvetica Neue\", Arial, \"Liberation Sans\", FreeSans, sans-serif;"
"    font-weight: bold;"
"}"
"#navigation > li > span, #navigation > li > a {"
"    display: inline-block;"
"    padding: 5px 10px;"
"    text-decoration: none;"
"    font-family: \"Helvetica Neue\", Arial, \"Liberation Sans\", FreeSans, sans-serif;"
"    font-weight: bold;"
"}"
"ul {"
"    -webkit-margin-before: 0px;"
"    -webkit-margin-after: 0px;"
"    -webkit-margin-start: 0px;"
"    -webkit-margin-end: 0px;"
"    -webkit-padding-start: 40px;"
"}"
"li {"
"    margin-left: 30px;"
"}"
"#navigation span {"
"    -moz-border-radius-bottomleft: 5px;"
"    -moz-border-radius-topright: 5px;"
"    -webkit-border-bottom-left-radius: 5px;"
"    -webkit-border-top-right-radius: 5px;"
"}"
"#navigation a {"
"    -moz-border-radius-bottomleft: 5px;"
"    -moz-border-radius-topright: 5px;"
"    -webkit-border-bottom-left-radius: 5px;"
"    -webkit-border-top-right-radius: 5px;"
"    color: white;"
"    text-decoration: underline;"
"    cursor: auto;"
"}"
".box {"
"    background: #fff;"
"    border: 1px solid;"
"    padding: 5px 0px;"
"    background: #fff;"
"    border-color: #eee;"
"    margin-bottom: 10px;"
"    -moz-border-radius-bottomright: 5px;"
"    -moz-border-radius-topleft: 5px;"
"    -moz-border-radius-bottomleft: 5px;"
"    -moz-border-radius-topright: 5px;"
"    -webkit-border-bottom-right-radius: 5px;"
"    -webkit-border-top-left-radius: 5px;"
"    -webkit-border-bottom-left-radius: 5px;"
"    -webkit-border-top-right-radius: 5px;"
"}"
"table {"
"    width: 100%;"
"}"
"th, td {"
"    padding: 7px 15px;"
"    text-align: left;"
"    color: #333;"
"    font: 13px/1.5 'Helvetica Neue', Arial, 'Liberation Sans', FreeSans, sans-serif;"
"    background-image: linear-gradient("
"            rgb(250, 250, 250), rgb(255, 255, 255)"
"    );"
"}"
"#foot {"
"    background: #313230;"
"    -moz-border-radius-topright: 10px;"
"    border: 0;"
"    margin: 10px auto 0 auto;"
"    padding: 5px 30px;"
"}"
"/* Underline From Center */"
".hvr-underline-from-center {"
"    display: inline-block;"
"    vertical-align: middle;"
"    -webkit-transform: perspective(1px) translateZ(0);"
"    transform: perspective(1px) translateZ(0);"
"    box-shadow: 0 0 1px transparent;"
"    position: relative;"
"    overflow: hidden;"
"}"
".hvr-underline-from-center:before {"
"    content: \"\";"
"    position: absolute;"
"    z-index: -1;"
"    left: 50%;"
"    right: 50%;"
"    bottom: 0;"
"    background: #2098D1;"
"    height: 4px;"
"    -webkit-transition-property: left, right;"
"    transition-property: left, right;"
"    -webkit-transition-duration: 0.3s;"
"    transition-duration: 0.3s;"
"    -webkit-transition-timing-function: ease-out;"
"    transition-timing-function: ease-out;"
"}"
".hvr-underline-from-center:hover:before, .hvr-underline-from-center:focus:before, .hvr-underline-from-center:active:before {"
"    left: 0;"
"    right: 0;"
"}"
""
"/* switch */"
"label.input-toggle {"
"    line-height: 0;"
"    font-size: 0;"
"    display: inline-block;"
"    margin: 0;"
"}"
""
"label.input-toggle > span {"
"    display: inline-block;"
"    position: relative;"
"    background-image: -webkit-linear-gradient(left, #848484 0%, #848484 50%, #7cbcbf 50%, #7cbcbf 100%);"
"    background-image: linear-gradient(to right, #848484 0%, #848484 50%, #7cbcbf 50%, #7cbcbf 100%);"
"    background-size: 64px 14px;"
"    background-position: 0%;"
"    border-radius: 32px;"
"    width: 32px;"
"    height: 14px;"
"    cursor: pointer;"
"    -webkit-transition: background-position 0.2s ease-in;"
"    transition: background-position 0.2s ease-in;"
"}"
""
"label.input-toggle > input:checked + span { background-position: -100%; }"
""
"label.input-toggle > span:after {"
"    content: \"\";"
"    display: block;"
"    position: absolute;"
"    width: 18px;"
"    height: 18px;"
"    background: #F1F1F1;"
"    -webkit-align-self: center;"
"    -ms-flex-item-align: center;"
"    align-self: center;"
"    top: 50%;"
"    left: 0;"
"    -webkit-transform: translateY(-50%);"
"    transform: translateY(-50%);"
"    border-radius: 100%;"
"    box-shadow: 0 1px 3px rgba(0, 0, 0, 0.5);"
"    -webkit-transition: left 0.2s ease-in, background-color 0.2s ease-in, -webkit-transform 0.3s ease;"
"    transition: left 0.2s ease-in, background-color 0.2s ease-in, -webkit-transform 0.3s ease;"
"    transition: left 0.2s ease-in, background-color 0.2s ease-in, transform 0.3s ease;"
"    transition: left 0.2s ease-in, background-color 0.2s ease-in, transform 0.3s ease, -webkit-transform 0.3s ease;"
"}"
""
"label.input-toggle > input:checked + span:after {"
"    left: calc(100% - 18px);"
"    background-color: #1D8B90;"
"}"
""
"label.input-toggle > span:active:after {"
"    -webkit-transform: translateY(-50%) scale3d(1.15, 0.85, 1);"
"    transform: translateY(-50%) scale3d(1.15, 0.85, 1);"
"}"
""
"label.input-toggle > input:disabled + span:active:after {"
"    -webkit-transform: translateY(-50%);"
"    transform: translateY(-50%);"
"}"
""
"label.input-toggle > input:disabled + span { cursor: default; }"
""
"label.input-toggle > input:disabled + span { background: #D5D5D5; }"
""
"label.input-toggle > input:disabled + span:after { background: #BDBDBD; }"
""
"label.input-toggle > input {"
"    display: block;"
"    position: absolute;"
"    opacity: 0;"
"    width: 0;"
"    height: 0;"
"}"
};

/* heater_js*/
const char heater_js[] PROGMEM = {
"var startManualForm;"
"var manualOffForm;"
"var stopManualForm;"
"var counter = 0;"
""
"function load() {"
""
"    alert('load heater.js');"
"    document.getElementById('heater').onsubmit = function (event) {"
"        event.preventDefault();"
"        sendPost(this, commandResponse);"
"    };"
"    startManualForm = document.getElementById('startManualForm');"
"    startManualForm.onsubmit = function () {"
"        event.preventDefault();"
"        sendPost(this, commandResponse);"
"    };"
"    manualOffForm = document.getElementById('manualOffForm');"
"    manualOffForm.onsubmit = function () {"
"        event.preventDefault();"
"        sendPost(this, commandResponse);"
"    };"
"    stopManualForm = document.getElementById('stopManualForm');"
"    stopManualForm.onsubmit = function () {"
"        event.preventDefault();"
"        sendPost(this, commandResponse);"
"    };"
"    startManualForm.style.display = 'none';"
"    manualOffForm.style.display = 'none';"
"    stopManualForm.style.display = 'none';"
""
"    getJson(heaterStatusPath, refreshFunction);"
""
"    setInterval(function(){"
"        document.getElementById('timer').innerHTML = counter++;"
"        if (counter % 20 == 0)"
"            getJson(heaterStatusPath, refreshFunction); }, 1000);"
"}"
""
"function commandResponse(json) {"
"    document.getElementById('command').innerHTML += 'command result' + JSON.stringify(json);"
"    getJson(heaterStatusPath, refreshFunction);"
"}"
""
"var refreshFunction = function refresh(json) {"
"    document.getElementById('summary').innerHTML = JSON.stringify(json);"
""
"    if (json.enabled)"
"        document.getElementById('heaterEnabled').checked = true;"
"    else"
"        document.getElementById('heaterEnabled').checked = false;"
""
"    var pinSelectControl = document.getElementById('pinSelect');"
"    for (var i, j = 0; i = pinSelectControl.options[j]; j++) {"
"        if (i.value == json.pin) {"
"            pinSelectControl.selectedIndex = j;"
"            break;"
"        }"
"    }"
"    if (json.status == 'manual' || json.status == 'manualoff') {"
"        startManualForm.style.display = 'none';"
"        manualOffForm.style.display = 'none';"
"        stopManualForm.style.display = 'block';"
""
"    } else {"
"        startManualForm.style.display = 'block';"
"        manualOffForm.style.display = 'block';"
"        stopManualForm.style.display = 'none';"
"    }"
"};"
""
"function sendPost(form, callback) {"
"    startManualForm.style.display = 'block';"
"    manualOffForm.style.display = 'block';"
"    stopManualForm.style.display = 'block';"
"    var data = formInputToJSON(form);"
"    sendCommand(data, callback);"
"}"
};

/* index_js*/
const char index_js[] PROGMEM = {
"function load() {"
""
"    alert('load');"
""
"    var powerCheckBox = document.getElementById('powerCheckBox').onclick = onClickPowerCheckBoxFunction;"
"    getJson(settingsPath, refreshFunction);"
""
"    document.getElementById('popupButton').onclick = provaFunction;"
""
"}"
""
"var provaFunction = function prova() {"
""
"    popFunction(\"prova\",function () {"
"        alert('ok');"
"    },null/*function () {"
"        alert('cancel');"
"    }*/)"
"};"
""
"var refreshFunction = function refresh(json) {"
"    document.getElementById('summary').innerHTML = JSON.stringify(json);"
"    if (json.hasOwnProperty(\"power\")) {"
"        if (json.power == \"on\")"
"            document.getElementById('powerCheckBox').checked = true;"
"        else"
"            document.getElementById('powerCheckBox').checked = false;"
"    }"
"};"
""
"function sendPost(form, callback) {"
"    var data = formInputToJSON(form);"
"    sendCommand(data, callback);"
"}"
""
"var onClickPowerCheckBoxFunction = function onClickPowerCheckBox() {"
""
"    var jsonString;"
"    if ( this.checked == true ) {"
"        alert( \"Accendere il dispositivo?\" );"
"        jsonString = '{\"command\" : \"power\", \"status\" : \"on\"}';"
"    } else {"
"        alert( \"Spengere il dispositivo?\" );"
"        var jsonString = '{\"command\" : \"power\", \"status\" : \"off\"}';"
"    }"
""
"    var json =  JSON.parse(jsonString);"
"    sendCommand(json, refreshFunction);"
""
"}"
};

/* settings_js*/
const char settings_js[] PROGMEM = {
""
"function load() {"
""
"    document.getElementById('shield').onsubmit = function (event) {"
"        event.preventDefault();"
"        sendPost(this, commandResponse);"
"    };"
"    getJson(settingsPath, refreshFunction);"
"}"
""
"function commandResponse(json) {"
"    document.getElementById('command').innerHTML += 'command result' + JSON.stringify(json);"
"    getJson(settingsPath, refreshFunction);"
"}"
""
"var refreshFunction = function refresh(json) {"
"    document.getElementById('summary').innerHTML = JSON.stringify(json);"
""
"    document.getElementById('localport').value = json.localport;"
"    document.getElementById('shieldname').value = json.shieldname;"
"    document.getElementById('ssid').value = json.ssid;"
"    document.getElementById('password').value = json.password;"
"    document.getElementById('servername').value = json.servername;"
"    document.getElementById('serverport').value = json.serverport;"
"};"
""
"function sendPost(form, callback) {"
"    var data = formInputToJSON(form);"
"    sendCommand(data, callback);"
"}"
};

/* webduino_js*/
const char webduino_js[] PROGMEM = {
"var serverPath = '';"
""
"var heaterStatusPath = serverPath+'/heaterstatus';"
"var commandPath = serverPath+'/command';"
"var settingsPath = serverPath+'/settings';"
""
"var popFunction = function pop(modalText,okCallback,cancelCallback) {"
""
"    document.getElementById('modalPopup').style.display = 'block';"
"    document.getElementById('modalText').innerHTML = modalText;"
""
"    var okButton = document.getElementById('okButton');"
"    var cancelButton = document.getElementById('cancelButton');"
"    if (okCallback != null) {"
"        document.getElementById('okButton').style.display = 'block';"
"        document.getElementById('okButton').onclick = okCallback;"
"    } else {"
"        document.getElementById('okButton').style.display = 'none';"
"    }"
""
"    var span = document.getElementsByClassName(\"close\")[0];"
"    if (cancelCallback != null) {"
"        document.getElementById('cancelButton').style.display = 'block';"
"        document.getElementById('cancelButton').onclick = cancelCallback;"
""
"        /* When the user clicks on <span> (x), close the modal*/"
"        span.style.display = 'block';"
"        span.onclick = function() {"
"            hideFunction();"
"        };"
"        /* When the user clicks anywhere outside of the modal, close it*/"
"        window.onclick = function(event) {"
"            if (event.target == modal) {"
"                hideFunction();"
"            }"
"        };"
""
"    } else {"
"        span.style.display = 'none';"
"        document.getElementById('cancelButton').style.display = 'none';"
"    }"
"};"
""
"var hideFunction = function hide() {"
"    document.getElementById('modalPopup').style.display = 'none';"
"};"
""
"/*To detect escape button*/"
"document.onkeydown = function(evt) {"
"    evt = evt || window.event;"
"    if (evt.keyCode == 27) {"
"        hideFunction('popDiv');"
"    }"
"};"
""
"function getJson(path, callback) {"
"    popFunction('Aggiornamento...' /*,function () {"
"     alert('ok');"
"     }*/);"
"    var request = new XMLHttpRequest();"
"    request.open('GET', path, true);"
"    request.setRequestHeader(\"X-Requested-With\", \"XMLHttpRequest\");"
"    request.onload = function () {"
"        if (this.status >= 200 && this.status < 400) {"
"            var json = JSON.parse(this.response);"
"            callback(json);"
"        } else {"
"            alert('errore:'+this.status);"
""
"        }"
"        hideFunction();"
"    };"
"    request.send();"
"}"
""
"function formInputToJSON(form) {"
"    var data = {};"
"    for (var i = 0, ii = form.length; i < ii; ++i) {"
"        var input = form[i];"
"        if (input.name) {"
""
"            if (input.type == 'number') {"
"                data[input.name] = Number(input.value);"
"            }"
"            else if (input.type == 'radio' && input.checked) {"
"                if (input.value == '0')"
"                    data[input.name] = false;"
"                else"
"                    data[input.name] = true;"
"            }"
"            else if (input.type == 'checkbox') {"
"                if (input.checked)"
"                    data[input.name] = true;"
"                else"
"                    data[input.name] = false;"
"            }"
"            else {"
"                data[input.name] = input.value;"
"            }"
"        }"
"    }"
"    return data;"
"}"
""
"function sendCommand(data, callback) {"
""
"    var xhr = new XMLHttpRequest();"
"    xhr.open('POST', commandPath, true);"
"    xhr.setRequestHeader('Content-Type', 'application/json; charset=UTF-8');"
"    xhr.send(JSON.stringify(data));"
"    document.getElementById('command').innerHTML = 'command sent' + JSON.stringify(data);"
"    xhr.onloadend = function () {"
"        hideFunction();"
"        result = xhr.responseText;"
"        var json = JSON.parse(result);"
"        callback(json);"
"    };"
"    popFunction('Invio comando in corso...' /*,function () {"
"        alert('ok');"
"    }*/);"
"}"
};
