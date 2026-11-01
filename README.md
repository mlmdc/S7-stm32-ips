<p align="center">
  <img src="images/1-13-044-0XX_DC_Motor_51x73.png" alt="Moteur d'exemple" width="70%">
</p>

<h1 align="center">STM32 IPS</h1>

<p align="left">
  <strong>🇬🇧 EN - </strong> STM32F411RE firmware and notebooks for studying speed control and measurement on a permanent-magnet DC motor.<br>
  <strong>🇫🇷 FR - </strong> Firmware STM32F411RE et notebooks dédiés à l’étude de la commande et de la mesure de vitesse d’un moteur à courant continu à aimants permanents.
</p>

[Back to portfolio](https://mlmdc.github.io/mlmdc_portfolio/) · [Contents / Sommaire](#contents--sommaire) · [Rapport / Report](C-5-article.pdf)

## Contents / Sommaire

- [Overview / Présentation](#overview--présentation)
- [Problem and approach / Problématique et approche](#problem-and-approach--problématique-et-approche)
- [Firmware methods / Méthodes du firmware](#firmware-methods--méthodes-du-firmware)
- [Technical decisions / Choix techniques](#technical-decisions--choix-techniques)
- [Experiments and limits / Expériences et limites](#experiments-and-limits--expériences-et-limites)
- [Getting started / Démarrage](#getting-started--démarrage)
- [Repository structure / Structure du dépôt](#repository-structure--structure-du-dépôt)
- [Acknowledgments / Remerciements](#acknowledgments--remerciements)
- [License](#license)

## Overview / Présentation

🇬🇧 **EN -** This repository combines STM32 firmware with notebooks for modeling a motor and processing serial measurements. On the STM32F411RE, the firmware estimates shaft speed from an incremental encoder, runs a discrete PID controller, and drives a motor-driver input with PWM. An ADC input measures the voltage connected to the current-sense shunt, while USART2 carries telemetry and fixed-size commands. The notebooks contain unloaded and loaded motor simulations, serial acquisition routines, and data post-processing; they are supporting study materials rather than a documented quantitative validation of the firmware.

<br>

🇫🇷 **FR -** Ce dépôt rassemble le firmware du STM32F411RE et des notebooks consacrés à la modélisation du moteur et au traitement des mesures série. Le firmware estime la vitesse à partir d’un encodeur incrémental, applique un PID discret et pilote le moteur en PWM. Il acquiert également une tension liée au shunt et échange télémétrie et commandes via USART2. Les notebooks proposent des simulations à vide et en charge, ainsi que des outils d’acquisition et de traitement. Ils servent de supports d’étude et ne constituent pas une validation quantitative du firmware.

## Problem and approach / Problématique et approche

<p align="left">
  <img src="images/compar_syst_moteur_libre_charge.png" alt="Moteur d'exemple" width="70%">
</p>

<p><em>FR : Comparaison de modèles dynamiques à vide et en charge. <br> EN: Comparison of dynamic models under no-load and loaded conditions.</em></p>

🇬🇧 **EN -** The firmware brings speed feedback, motor actuation, and auxiliary voltage acquisition together on a microcontroller. Its main inputs are encoder counts, the ADC voltage, a speed reference held by the firmware, and UART commands. A timer schedules the speed estimate and controller update; the controller output is converted to PWM duty cycle, while the shunt-voltage measurement is sent as telemetry. Motor direction is controlled separately from the speed loop.

The notebooks address the same system from two complementary angles: `SIM_Moteur.ipynb` and `SIM_MOTEUR_MOTEURS.ipynb` simulate motor and controller behavior under different model and sampling assumptions; `stm32_com.ipynb` explores serial communication, data acquisition, and post-processing. These models can help formulate experiments, but their outputs do not establish how the physical motor performs.

<br>

🇫🇷 **FR -** Le firmware regroupe sur un même microcontrôleur le retour de vitesse, la commande du moteur et l’acquisition d’une tension auxiliaire. Il exploite les comptes de l’encodeur, la mesure ADC, une consigne définie dans le code et les commandes UART. Un timer cadence le calcul de vitesse et le PID, dont la sortie pilote le PWM. La tension du shunt est transmise en télémétrie ; le sens de rotation est géré séparément.

Les notebooks complètent cette approche : `SIM_Moteur.ipynb` et `SIM_MOTEUR_MOTEURS.ipynb` simulent le moteur et le correcteur selon plusieurs hypothèses, tandis que `stm32_com.ipynb` traite la communication série et les mesures. Ces simulations préparent les essais, mais ne décrivent pas à elles seules le comportement du moteur réel.

```mermaid
flowchart LR
    Encoder[Motor encoder] -->|TIM1 counts| Speed[Speed estimate]
    Speed --> PID[Discrete PID<br/>10 ms update]
    Reference[Speed reference<br/>held in firmware] --> PID
    PID -->|0–24 V command mapped to duty| PWM[TIM3 PWM]
    Direction[UART direction command] --> Driver[Motor driver]
    PWM --> Driver
    Driver --> Motor[DC motor]
    Motor --> Encoder
    Shunt[Shunt voltage] -->|ADC1 + DMA| Filter[Filtered voltage]
    Filter --> Telemetry[USART2 telemetry]
    Speed --> Telemetry
    PWM --> Telemetry
    UART[USART2 commands] --> Firmware[STM32F411RE firmware]
    Firmware --> PID
    Firmware --> PWM
    Firmware --> Direction
```


## Firmware methods / Méthodes du firmware

🇬🇧 **EN :**

- **Speed feedback:** TIM1 counts the quadrature encoder. Every 10 ms, the firmware converts the count change to a speed in RPM using a hard-coded 2,000-counts-per-revolution assumption.
- **Control and actuation:** the PID uses the difference between its stored reference and the absolute measured speed. Its discrete difference equation updates a voltage command, clamped to 0–24 V and mapped linearly to TIM3 channel 1 PWM duty cycle. The direction output is handled separately.
- **Voltage acquisition:** ADC1 channel 0 (PA0) runs with DMA. The firmware keeps eight samples, converts them to a nominal 0–3.3 V range, and applies an exponential moving average with alpha 0.5. This is a filtered voltage reading, not a calibrated current measurement.
- **Serial interface:** USART2 (PA2/PA3) transmits time, ADC voltage, PWM value, and speed. Reception uses 16-byte ASCII frames: the first four bytes identify a command and the remaining bytes contain its value. Implemented commands start or stop the timer interrupt (`0x01`, `0x02`), enable or disable PWM (`0x03`, `0x04`), set duty cycle or voltage (`0x05`, `0x07`), set direction (`0x06`), and write PID gain variables (`0x0B`, `0x0D`, `0x0F`).

<br>

🇫🇷 **FR :**

- **Retour de vitesse :** TIM1 compte les impulsions de l’encodeur en quadrature. Toutes les 10 ms, le firmware calcule la vitesse en tr/min à partir de la variation du compteur, avec une hypothèse codée de 2 000 comptes par tour.
- **Commande moteur :** le PID compare la consigne mémorisée à la valeur absolue de la vitesse mesurée. Sa sortie, limitée à 0–24 V, est convertie en rapport cyclique PWM sur TIM3 canal 1. Le sens de rotation est piloté séparément.
- **Acquisition de tension :** ADC1 (canal 0, PA0) échantillonne avec DMA. Huit échantillons sont convertis dans une plage nominale de 0 à 3,3 V, puis filtrés par moyenne exponentielle (alpha = 0,5). Cette mesure de tension n’est pas une mesure de courant étalonnée.
- **Liaison série :** USART2 (PA2/PA3) transmet le temps, la tension ADC, le PWM et la vitesse. Les trames ASCII de 16 octets contiennent quatre octets d’identification, suivis de la valeur. Les commandes gèrent le timer (`0x01`, `0x02`), le PWM (`0x03`, `0x04`), le rapport cyclique ou la tension (`0x05`, `0x07`), le sens (`0x06`) et les gains du PID (`0x0B`, `0x0D`, `0x0F`).

## Technical decisions / Choix techniques

🇬🇧 **EN :**
- The firmware is an STM32Cube HAL project generated/configured with STM32CubeMX 6.8.0. TIM1 handles encoder counting, TIM2 schedules the 10 ms control update, TIM3 generates PWM, and USART2 is configured for 115200 baud, 8N1. The `.ioc` file is the source for inspecting the peripheral configuration.
- The controller output is non-negative and limited to 24 V-equivalent command; speed is compared by magnitude and direction is a separate GPIO signal. This is not a signed, bidirectional speed controller.
- UART gain commands update `Kp`, `Ki`, and `Kd`, but the discrete PID coefficients are calculated only during initialization. Changing gains at runtime therefore does not update the coefficients used by the control equation.
- The encoder conversion assumes 2,000 counts per revolution and a 10 ms measurement interval. That assumption must match the encoder, its wiring, and any mechanical gearing before RPM values can be interpreted as calibrated measurements.
- The notebooks use nominal motor-model parameters (including inductance, resistance, motor constant, and inertia) as simulation inputs. The repository does not document these as experimentally identified values. The ADC-to-voltage scaling likewise has no documented calibration of the complete shunt and analog measurement chain.

<br>

🇫🇷 **FR :**
- Le firmware STM32Cube HAL est configuré avec STM32CubeMX 6.8.0 : TIM1 compte l’encodeur, TIM2 cadence la commande à 10 ms, TIM3 génère le PWM et USART2 communique en 115200 bauds, 8N1. La configuration des périphériques est disponible dans le fichier `.ioc`.
- La commande est limitée à une valeur positive équivalente à 24 V. La vitesse est traitée en valeur absolue et le sens est piloté par un GPIO distinct : le système ne gère pas de consigne signée bidirectionnelle.
- Les commandes UART modifient `Kp`, `Ki` et `Kd`, mais les coefficients du PID discret ne sont calculés qu’au démarrage. Les changements de gains à l’exécution ne sont donc pas pris en compte par l’équation de commande.
- Le calcul de vitesse repose sur 2 000 comptes par tour et une période de 10 ms. Ces paramètres doivent correspondre à l’encodeur, à son câblage et à la réduction mécanique avant d’interpréter les tr/min comme une mesure étalonnée.
- Les simulations utilisent des paramètres nominaux du moteur, sans identification expérimentale documentée. La conversion ADC-tension ne s’appuie pas non plus sur un étalonnage documenté du shunt et de la chaîne analogique.

## Experiments and limits / Expériences et limites

<p align="left">
  <img src="images/exemple_951_mesures_mcv_controle.png" alt="Exemple de commande en vitesse du moteur, à vide et en charge." width="70%">
</p>

<p><em>FR : Exemple de commande en vitesse du moteur, à vide et en charge. <br> EN: Example of motor speed control under no-load and loaded conditions.</em></p>


🇬🇧 **EN -** The repository demonstrates an integrated firmware path for encoder feedback, PWM output, ADC voltage acquisition, and UART telemetry. The simulation notebooks contain saved outputs for modeled unloaded/loaded cases and different sample periods; these are results of the notebook models, not measurements of the assembled motor. `stm32_com.ipynb` contains serial measurement and post-processing routines, but the repository does not provide a documented, calibrated data set or a quantitative comparison sufficient to validate controller performance or rank the methods.

There is also an important interface limitation: the PID reference is initialized to zero and is not set by any of the implemented UART commands. UART can change direct PWM/voltage, direction, and gain variables, but it does not provide a speed-reference command. Further validation would require exposing and documenting that reference, checking the encoder scale and sensor calibration, and recording repeatable motor tests with their hardware and load conditions.

<br>

🇫🇷 **FR -** Le firmware intègre le retour d’encodeur, la commande PWM, l’acquisition ADC et la télémétrie UART. Les résultats sauvegardés dans les notebooks sont des simulations à vide et en charge, avec différentes périodes d’échantillonnage — pas des mesures du moteur assemblé. `stm32_com.ipynb` propose des outils d’acquisition et de traitement, mais le dépôt ne contient ni données étalonnées ni comparaison quantitative permettant de valider le correcteur ou de comparer les méthodes.

La consigne du PID est initialisée à zéro et ne peut pas être modifiée par UART. L’interface permet de régler le PWM ou la tension, le sens et les gains, mais pas la vitesse de consigne. Une validation complète suppose donc d’ajouter cette commande, de vérifier l’échelle de l’encodeur et l’étalonnage des capteurs, puis de réaliser des essais répétables avec le matériel et la charge documentés.

## Getting started / Démarrage

### Firmware

🇬🇧 **EN -** Requirements: STM32CubeIDE with STM32F4 device support, an STM32F411RE target, an ST-LINK programmer/debugger, and a motor, encoder, and driver wired to the project configuration. For serial communication, use a 3.3 V-compatible USB-to-UART adapter on USART2 (PA2/PA3).

The diagram shows how the hardware and notebooks fit together: the simulation notebooks use a motor model, while `stm32_com.ipynb` can exchange commands and telemetry with the running firmware over UART.

```mermaid
flowchart LR
    IDE[STM32CubeIDE] -->|Build and flash via ST-LINK| FW[STM32F411RE firmware]
    FW -->|PWM and direction| Driver[Motor driver]
    Driver --> Motor[DC motor]
    Motor -->|Encoder feedback| FW
    FW -->|Telemetry and commands| UART[USB-to-UART adapter]
    UART <-->|Read telemetry and send commands| SerialNB[stm32_com.ipynb]
    SimNB[Simulation notebooks] <-->|Simulated inputs and responses| Model[Motor and controller model]
```

1. Open STM32CubeIDE and import `MCC_IPS/MCC_IPS_FINISHED` using **File → Import → Existing Projects into Workspace**.
2. Build the `MCC_IPS_FINISHED` project, connect the board through ST-LINK, and use **Run → Debug** (or **Run**) to program it.
3. To inspect the peripheral setup, open `MCC_IPS/MCC_IPS_FINISHED/MCC_IPS_FINISHED.ioc` in STM32CubeMX. Regenerate code only when needed because generated files also contain user code.
4. Configure the serial monitor for **115200 baud, 8 data bits, no parity, 1 stop bit (8N1)**.


#### Serial example

The following example sends the 16-byte `0x01` frame, which enables the timer interrupt, then reads one telemetry line. Replace `COM3` with the port assigned to the adapter. The firmware starts the timer during initialization as well; this command re-enables its interrupt if it has been stopped.

```console
python -m pip install pyserial
```

```python
import serial

with serial.Serial("COM3", 115200, timeout=1) as port:
    port.write(b"0x01" + b";" * 12)
    print(port.readline().decode(errors="replace").strip())
```

### Notebooks

🇬🇧 **EN -** The notebooks require Jupyter and import `numpy`, `python-control` (the `control` module), `control_plotly`, `matplotlib`, and/or `pyserial`, depending on the notebook. No dependency manifest or pinned versions are provided. Once a compatible environment is available, start a notebook from the repository root, for example:

```console
python -m jupyter lab SIM_Moteur.ipynb
```

The serial notebook also expects a connected board and a correctly configured serial port.

## Repository structure / Structure du dépôt

| Path | Purpose / Rôle |
| --- | --- |
| `MCC_IPS/MCC_IPS_FINISHED/Core/Src/main.c` | Firmware initialization and 10 ms control callback / Initialisation et callback de commande à 10 ms |
| `MCC_IPS/MCC_IPS_FINISHED/Core/Src/speed.c` | Encoder-based speed estimate / Estimation de vitesse par encodeur |
| `MCC_IPS/MCC_IPS_FINISHED/Core/Src/pidFilter.c` | Discrete PID implementation / Implémentation du PID discret |
| `MCC_IPS/MCC_IPS_FINISHED/Core/Src/pwmCommand.c` | PWM and voltage-to-duty conversion / PWM et conversion tension-rapport cyclique |
| `MCC_IPS/MCC_IPS_FINISHED/Core/Src/current.c` | ADC sampling and voltage filtering / Acquisition ADC et filtrage de tension |
| `MCC_IPS/MCC_IPS_FINISHED/Core/Src/comm.c` | UART telemetry and command parsing / Télémétrie UART et analyse des commandes |
| `MCC_IPS/MCC_IPS_FINISHED/MCC_IPS_FINISHED.ioc` | STM32CubeMX peripheral configuration / Configuration des périphériques STM32CubeMX |
| `SIM_Moteur.ipynb`, `SIM_MOTEUR_MOTEURS.ipynb` | Motor and controller simulations / Simulations du moteur et du correcteur |
| `stm32_com.ipynb` | Serial acquisition and data post-processing / Acquisition série et post-traitement |

## Acknowledgments / Remerciements

**EN** The initial code base was provided by M. Kerhoas: [DC motor control drivers](https://web.enib.fr/~kerhoas/asserv_mcc_drivers.html).

**FR** Le code initial a été fourni par M. Kerhoas : [drivers de commande de moteur à courant continu](https://web.enib.fr/~kerhoas/asserv_mcc_drivers.html).

## License

This project is licensed under the [MIT License](LICENSE).
