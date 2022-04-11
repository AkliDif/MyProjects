# Bale

Ce projet est un projet de programation robotique.

Le but de ce projet est de programmé un robot Thymio II afin de jouer au mini-golf sur une piste donnée.

Le principe de jeu :
    le robot doit suivre une ligne de piste droite (noire) qu'il capte avec un capteur infra-rouge situé sous le robot.
    sur cette piste,il trouvera deux code barre qu'il doit lire avec un deuxiéme capteur.
    le premier code barre indique la distance entre la fin de la piste et le green (le trou), et le deuxiéme indique dans quelle direction le green se situe.
    une fois les deux code barres sont lus, le robot tourne (ou pas), et avance vers le green on poussant avec lui une balle pour la rentré dans le green.
    (contrairement sur une vrai piste, sur le simulateur on ne peut pas ajouter les balles donc le robot va juste tourner et avancer vers le green en question)

Pour exécuter ce code, faut installer aseba studio et chargé la circuit trouvant dans le dossier circuit.
une fois le circuit est chargé sur aseba studio, il suffit de placer le robot sur une piste et appuyer sur le bouton d'avant sur le robot pour qu'il avance et marque un point dans le green de numero correspondant au numero de la piste choisie.
