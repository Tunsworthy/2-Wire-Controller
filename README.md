<h1>2-Wire LED Controller</h1>

<p>This project provides a firmware for a 2-wire LED controller based on the L293D mini H-bridge motor driver. The controller is designed to work with a 2-wire LED strip and is controllable via MQTT.</p>

<h2>Getting Started</h2>
<h3>Prerequisites</h3>
<ul>
    <li>Arduino IDE or PlatformIO</li>
    <li>An ESP8266-based development board, such as the NodeMCU v2 or the Wemos D1 mini</li>
    <li>A 2-wire LED strip</li>
    <li>An MQTT broker</li>
</ul>
<h3>Hardware Setup</h3>
<ol>
    <li>Connect the 2-wire LED strip to the L293D mini H-bridge motor driver as follows:
<ul>
    <li>LED strip positive (+) wire to INPUT_1</li>
    <li>LED strip negative (-) wire to INPUT_2</li>
</ul>
</li>
    <li>Connect the L293D mini H-bridge motor driver to the ESP8266-based development board as follows:
<ul>
    <li>ENABLE_A to D1 (GPIO5)</li>
    <li>INPUT_1 to D2 (GPIO4)</li>
    <li>INPUT_2 to D3 (GPIO0)</li>
</ul>
</li>
    <li>Power the development board and the LED strip.</li>
</ol>

<h3>Software Setup</h3>
<ol>
    <li>Clone or download this repository.</li>
    <li>Update the Wi-Fi and MQTT configurations in config.h to match your setup.</li>
    <li>Update the pin assignments in hardware.h to match your hardware configuration.</li>
    <li>Compile and upload the firmware to your development board using the Arduino IDE or PlatformIO.</li>
</ol>

<h2>Usage</h2>

<p>Once the firmware is uploaded to your development board and the hardware is set up, you can control the LED strip using MQTT messages. The firmware subscribes to the following topics:</p>
<ul>
    <li>
        <code>cmnd/&lt;device_id&gt;/power</code>: Controls the power state of the LED strip. Accepted values are ON and OFF.
    </li>
    <li>
     <code>cmnd/&lt;device_id&gt;/effect</code>: Controls the effect of the LED strip. Accepted values are:
        <ul>
            <li>Combination</li>
            <li>In Waves</li>
            <li>Sequential</li>
            <li>Slo Glo</li>
            <li>Chasing /Flash</li>
            <li>Twinkle</li>
            <li>SteadyOn</li>
            <li>Slow Fade Alternate</li>
        </ul>
    </li>
    <li>
        <code>tele/&lt;device_id&gt;/effect</code>: Returns the current effect
    </li>
    <li>
        <code>tele/&lt;device_id&gt;/status</code>: Returns the current power state
    </li>
</ul>

<h3>Examples</h3>
<ul>
    <li>
        To turn on the LED strip, publish the message ON to the topic <code>cmnd/&lt;device_id&gt;/power</code>.
    </li>
    <li>
        To set the effect of the LED strip to "slow glow", publish the message slowGlo to the topic <code>cmnd/&lt;device_id&gt;/effect</code>.
    </li>
</ul>