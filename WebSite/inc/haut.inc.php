<!Doctype html>
<html>
    <head>
        <title>Mon Site</title>
        <link rel="stylesheet" href="<?php echo RACINE_SITE; ?>inc/css/style.css" />
    </head>
    <body>    
        <header class="mainHead">                    
				<nav>
					<h1 class="logo">SportShop</h1>
					<?php
					if(internauteEstConnecteEtEstAdmin())
					{ 	

						echo '<ul>
						<li><a href="' . RACINE_SITE . 'admin/gestion_membre.php">Gestion des membres</a></li>';
						echo '<li> <a href="' . RACINE_SITE . 'admin/gestion_commande.php">Gestion des commandes</a></li>';
						echo '<li><a href="' . RACINE_SITE . 'admin/gestion_boutique.php">Gestion de la boutique</a></li>
						</ul>';
					}
					if(internauteEstConnecte())
					{
						echo '
						<ul>
						<li><a href="' . RACINE_SITE . 'profil.php">Profil</a></li>';
						echo '<li><a href="' . RACINE_SITE . 'boutique.php">Boutique</a></li>';
						echo '<li><a href="' . RACINE_SITE . 'panier.php">Panier</a></li>';
						echo '<li><a href="' . RACINE_SITE . 'connexion.php?action=deconnexion">Déconnexion</a></li>
						</ul>';
					}
					else
					{
						echo '<ul>
						<li><a href="' . RACINE_SITE . 'inscription.php">Inscription</a></li>';
						echo '<li><a href="' . RACINE_SITE . 'connexion.php">Connexion</a></li>';
						echo '<li><a href="' . RACINE_SITE . 'boutique.php">Boutique</a></li>';
						echo '<li><a href="' . RACINE_SITE . 'panier.php">Panier</a></li>
						</ul>';
					}

					?>
				</nav>
        </header>
        <section>
			<div class="conteneur"> 