#### Gimbal + encodeur (pas simple à trouver..seul site chelou qui le propose de ce que j'ai vu):

- http://fr.aliexpress.com/item/Dhl-libre-3-PCS-iPower-moteur-GBM4108H-120T-Cardan-Moteur-avec-AS5048A-Codeur-pour-3-axes/32670733151.html
- http://fr.aliexpress.com/item/iPower-GBM5208hH-200T-bruless-gimbal-motor-with-Magnetic-encoder-for-alexmos-basecam-electronics-gimbal-controller/32601885396.html

#### Lire le capteur (AS5048A est relativement populaire):

- http://ams.com/eng/Products/Magnetic-Position-Sensors/Angle-Position-On-Axis/AS5048A
- https://github.com/ZoetropeLabs/AS5048A-Arduino

#### Magnets

- http://www.first4magnets.com/circular-disc-rod-magnets-c34/6mm-o-d-x-3mm-i-d-x-1mm-thick-diametrically-magnetised-n42-neodymium-magnet-p3524
- http://www.first4magnets.com/circular-disc-rod-magnets-c34/6mm-dia-x-1mm-thick-diametrically-magnetised-n42-neodymium-magnet-p2681
- http://www.first4magnets.com/circular-disc-rod-magnets-c34/8mm-dia-x-1mm-thick-diametrically-magnetised-n42-neodymium-magnet-p2986
- http://www.first4magnets.com/other-c89/50mm-x-50mm-small-magnetic-field-viewing-paper-p3783

#### Carte control brushless (étrangement pas simple à trouver):
chip: http://www.st.com/content/ccc/resource/technical/document/application_note/78/44/47/d5/a8/63/4a/8e/CD00004062.pdf/files/CD00004062.pdf/jcr:content/translations/en.CD00004062.pdf

- http://manton.ca/open:bldc-motor-driver
- https://www.tindie.com/products/manton/brushless-dc-bldc-motor-driver/
or
- http://www.drotek.com/shop/en/home/212-brushless-gimbal-controller-l6234.html



#### Control gimbal à faible vitesse:

- http://www.berryjam.eu/2015/04/driving-bldc-gimbals-at-super-slow-speeds-with-arduino/
(utilise le chip ci-dessus)

Plus une arduino et une alim, je pense que c'est jouable pour un proto:

- https://www.proto-pic.co.uk/pololu-5v-1a-step-down-voltage-regulator-d24v10f5.html
- https://www.proto-pic.co.uk/arduino-pro-mini-328-5v16mhznew.html
- https://www.proto-pic.co.uk/protoshield-mini-for-arduino.html
- https://www.proto-pic.co.uk/ftdi-voltage-selectable-board.html


Je suis hyper chaud de me lancer, avec l'imprimante 3D du labo, je pense pouvoir faire un trucs bien intégré sous forme de module (avec p-t même la sortie en reductionx10)..

Je pense commander le moteur + capteur rapidement car long lead time.

**Problèmes:**
- moteur encore un peu gros:  45 x 25 mm pour le petit, faut tenter un module proto 48x48x36 (seul moteur pour l'instant avec capteur bien intégré..): http://www.robotshop.com/eu/fr/moteur-nacelle-stabilisee-ipower-gbm4108h-120t.html

**Questions:**
- Est-ce qu'il y a des doutes/objections sur le projet/l'idée?
- Est-ce que vous avez d'autres liens/sources d'info pour faire mieux/plus simple?
- Quel controlleur: [Michel Anton](https://www.tindie.com/products/manton/brushless-dc-bldc-motor-driver/) (cher, semble plus protégé) ou [Drotek](http://www.drotek.com/shop/en/home/212-brushless-gimbal-controller-l6234.html) (mois cher, plus petit)
