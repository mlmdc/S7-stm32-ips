# stm32_ips

Partie STM32 du projet d'asservissement d'un MCC du groupe C5.
Printemps 2024

## Objectifs

Plusieurs objectifs sont réalisés par la carte STM32 :
* Mesure du courant entrant dans le moteur (à l'aide d'une résistance de shunt) via l'ADC
* Nettoyage des mesures d'ADC pour exploitation
* Mesure de la vitesse de rotation du moteur (donnée par l'encodeur)
* Contrôle moteur basique : direction, vitesse, freinage
* Asservissement en vitesse du moteur par un filtre numérique interne
* Communication des valeurs mesurées via USART/UART

## Flexion

A l'issue du développement pour ce projet, le code sera intégralement réutilisé pour le projet Flexion XS du module CMV.
X raisons pour cette utilisation :
1. Mutualisation du développement
2. Retour qualité via M. Bourgeot
3. Ressemblance agréable entre les deux projets


## Authors and acknowledgment

Merci à M. Kerhoas pour la base de code
https://web.enib.fr/~kerhoas/asserv_mcc_drivers.html


