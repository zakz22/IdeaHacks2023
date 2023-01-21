#include <Arduino.h>

const char PAGE_MAIN[] PROGMEM = R"=====(

<!DOCTYPE html>
<html lang="en" class="js-focus-visible">

<head>
    <meta charset="UTF-8">
    <title>ESP32 Firebase Demo</title>
</head>

<body>
    <div class="container">
        <h1>Database Editor</h1>
        <div class="form">

            <form action="/get">
                Key: <input type="text" name="key">
                <br>Value: <input type="text" name="val">

                <div class="buttons">
                    <input type="submit" name="btn" value="READ">
                    <input type="submit" name="btn" value="WRITE">
                </div>
            </form>
        </div>
    </div>
</body>

</html>

)=====";   