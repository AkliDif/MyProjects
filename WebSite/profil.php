<?php
require_once("inc/init.inc.php");
//--------------------------------- TRAITEMENTS PHP ---------------------------------//
if(!internauteEstConnecte()) 
{
	header("location:connexion.php");
}

$contenu .= '<p class="centre">Bonjour <strong>' . $_SESSION['client']['nom'] . " " . $_SESSION['client']['prenom'] . '</strong></p>';
$contenu .= '<div class="cadre"><h2> Voici vos informations de profil </h2>';
$contenu .= '<p> votre email est: ' . $_SESSION['client']['email'] . '<br>';
$contenu .= 'votre ville est: ' . $_SESSION['client']['ville'] . '<br>';
$contenu .= 'votre cp est: ' . $_SESSION['client']['code_postal'] . '<br>';
$contenu .= 'votre adresse est: ' . $_SESSION['client']['adresse'] . '<br>';
$contenu .= '<a href ="update_profile.php"><input type="button" value="Modifier"> <a></p></div><br /><br />';
	
//--------------------------------- AFFICHAGE HTML ---------------------------------//
require_once("inc/haut.inc.php");
echo $contenu;
require_once("inc/bas.inc.php");
?>