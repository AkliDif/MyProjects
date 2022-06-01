<?php
require_once("inc/init.inc.php");
//--------------------------------- TRAITEMENTS PHP ---------------------------------//
//--- AFFICHAGE DES CATEGORIES ---//
$categories_des_produits = executeRequete("SELECT DISTINCT categorie FROM produit");
$contenu .= '<div class="boutique-gauche">';
$contenu .= "<ul>";
while($cat = $categories_des_produits->fetch_assoc())
{
	$contenu .= "<li><a href='?categorie="	. $cat['categorie'] . "'>" . $cat['categorie'] . "</a></li>";
}
$contenu .= "</ul>";
$contenu .= "</div>";
//--- AFFICHAGE DES PRODUITS ---//
$contenu .= '<div class="boutique-droite">';
if(isset($_GET['categorie']))
{
	$donnees = executeRequete("SELECT id_produit,reference,titre,photo,prix FROM produit WHERE categorie='$_GET[categorie]'");
	
	while($produit = $donnees->fetch_assoc())
	{
		$contenu .= '<div class="boutique-produit">';
		$contenu .= "<h3>$produit[titre]</h3>";
		$contenu .= "<a href=\"fiche_produit.php?id_produit=$produit[id_produit]\"><img src=\"$produit[photo]\" width=\"130\" height=\"100\" /></a>";
		$contenu .= "<p>$produit[prix] €</p>";
		$contenu .= '<a href="fiche_produit.php?id_produit=' . $produit['id_produit'] . '">Voir la fiche</a>';
		$contenu .= '</div>';
	}
}

if(isset ($_POST['filtre']))
{
	$typeFiltre = $_POST['filtre'];
	switch ($typeFiltre) :
		case "Triparmarque" : 
			$donnees = executeRequete("SELECT id_produit,reference,titre,photo,prix FROM produit ORDER BY marque ASC");	
			while($produit = $donnees->fetch_assoc())
			{
				$contenu .= '<div class="boutique-produit">';
				$contenu .= "<h3>$produit[titre]</h3>";
				$contenu .= "<a href=\"fiche_produit.php?id_produit=$produit[id_produit]\"><img src=\"$produit[photo]\" width=\"130\" height=\"100\" /></a>";
				$contenu .= "<p>$produit[prix] €</p>";
				$contenu .= '<a href="fiche_produit.php?id_produit=' . $produit['id_produit'] . '">Voir la fiche</a>';
				$contenu .= '</div>';
			}
		break;
	  
		case "Prixcroissant" :
			$donnees = executeRequete("SELECT id_produit,reference,titre,photo,prix FROM produit ORDER BY prix ASC");	
			while($produit = $donnees->fetch_assoc())
			{
				$contenu .= '<div class="boutique-produit">';
				$contenu .= "<h3>$produit[titre]</h3>";
				$contenu .= "<a href=\"fiche_produit.php?id_produit=$produit[id_produit]\"><img src=\"$produit[photo]\" width=\"130\" height=\"100\" /></a>";
				$contenu .= "<p>$produit[prix] €</p>";
				$contenu .= '<a href="fiche_produit.php?id_produit=' . $produit['id_produit'] . '">Voir la fiche</a>';
				$contenu .= '</div>';
			}
		break;
	  
		case "Prixdecroissant" :
			$donnees = executeRequete("SELECT id_produit,reference,titre,photo,prix FROM produit ORDER BY prix DESC");	
			while($produit = $donnees->fetch_assoc())
			{
				$contenu .= '<div class="boutique-produit">';
				$contenu .= "<h3>$produit[titre]</h3>";
				$contenu .= "<a href=\"fiche_produit.php?id_produit=$produit[id_produit]\"><img src=\"$produit[photo]\" width=\"130\" height=\"100\" /></a>";
				$contenu .= "<p>$produit[prix] €</p>";
				$contenu .= '<a href="fiche_produit.php?id_produit=' . $produit['id_produit'] . '">Voir la fiche</a>';
				$contenu .= '</div>';
			}
		break;

		default:
			break;
	  endswitch;
}

$contenu .= '</div>';
//--------------------------------- AFFICHAGE HTML ---------------------------------//
require_once("inc/haut.inc.php");

?>

<form method="post" >
<select name=filtre>
   <libellé>Tri</libellé>
   <option name="filtre" value="aucunfiltre"> Choisir un filtre</option>
   <option name="filtre" value="Triparmarque">Tri par marque</option>
   <option name="filtre" value="Prixcroissant">Prix croissant</option>
   <option name="filtre" value="Prixdecroissant">Prix décroissant</option>
</select>
<input type="submit" value="Filtrer" >
</form>

<?php

echo $contenu;

require_once("inc/bas.inc.php");
?>