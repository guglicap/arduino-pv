This library allows communication of an Arduino with a SunEzy inverter, connected via the serial port.

Functionality is mostly complete, the only missing feature (that I know of) is setting inverter parameters.

For details on the usage see the example.

Here's a table with all the status properties

| Property | Description              | Unit         |
|----------|--------------------------|--------------|
| temp     | Inverter Temperature     | 0.1 °C       |
| vpv      | Panel Voltage            | 0.1 V        |
| iac      | Grid Current             | 0.1 A        |
| vac      | Grid Voltage             | 0.1 V        |
| fac      | Grid Frequency           | 0.01 Hz      |
| pac      | Output Power             | 1 W          |
| zac      | Grid Impedance           | 1 mOhm       |
| etoday   | Accumulated Energy Today | 0.1 kWh      |
| etotal   | Accumulated Energy       | 0.1 kWh      |
| htotal   | Working Hours            | 1 h          |
| mode     | Operating Mode           | see status.h |
| error    | Error Code               | see status.h |

Based on https://archive.codeplex.com/?p=pv

Please note that, even though the python library has only been tested with a CMS-2000 inverter and a SunEzy 600E, it also worked on my SunEzy 2810, and it believe this would be the case for every SunEzy inverter, as long as it has a serial port and can be queried by the SunEzy Control software.
