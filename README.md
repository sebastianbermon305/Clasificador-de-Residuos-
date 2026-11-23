# Clasificador-de-Residuos-
Clasificador automático de canecas de reciclaje

## Problema que resuelve
La incorrecta separación de residuos en la fuente dificulta el reciclaje y aumenta la contaminación ambiental.

## Funcionamiento del sistema
1. El sistema recibe por comunicación serial (115200 baudios) el tipo de residuo detectado (`A` para Aprovechable, `O` para Orgánico, `N` para No Aprovechable).
2. El sistema entra en estado armado durante un máximo de 10 segundos esperando que el usuario acerque su mano u objeto.
3. Cuando el sensor ultrasónico detecta presencia entre 2 cm y 15 cm (confirmada con 3 lecturas consecutivas para evitar falsos positivos), abre suavemente la compuerta correspondiente, espera 2 segundos y vuelve a cerrarla.

## Componentes usados
* 1x Microcontrolador ESP32
* 3x Servomotores
* 1x Sensor ultrasónico HC-SR04
* Protoboard, cables jumper y fuente de alimentación externa para los servomotores.

## Integrantes
