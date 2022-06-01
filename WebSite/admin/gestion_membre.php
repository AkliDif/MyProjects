<?php

require_once("../inc/init.inc.php");
if(!internauteEstConnecteEtEstAdmin())
{
	header("location:connexion.php");
	exit();
}
if(isset($_GET['msg']) && $_GET['msg'] == "supok")
{
	executeRequete("DELETE FROM client WHERE id_client=$_GET[id_client]");
	header("location:gestion_membre.php");
}

if(isset($_GET['msg']) && $_GET['msg'] == "pass_admin")
{
	executeRequete("UPDATE client SET statut = 1 WHERE id_client=$_GET[id_client]");
	header("location:gestion_membre.php");
}

if(isset($_GET['msg']) && $_GET['msg'] == "supp_admin")
{
	executeRequete("UPDATE client SET statut = 0 WHERE id_client=$_GET[id_client]");
	unset ($_SESSION['client']);
	header("location:../boutique.php");
}
//-------------------------------------------------- Affichage ---------------------------------------------------------//
require_once("../inc/haut.inc.php");

echo '<h1> Voici les clients inscrits sur site </h1>';
	$resultat = executeRequete("SELECT * FROM client");
	echo "Nombre de membre(s) : " . $resultat->num_rows;
	echo "<table style='border-color:red' border=10> <tr>";
	while($colonne = $resultat->fetch_field())
	{    
		echo '<th>' . $colonne->name . '</th>';
	}
	echo '<th> Supprimer </th>';
	echo '<th> Admin </th>';
	echo "</tr>";
	while ($membre = $resultat->fetch_assoc())
	{
		echo '<tr>';
		foreach ($membre as $information)
		{
			echo '<td>' . $information . '</td>';
		}
		echo "<td><a href='gestion_membre.php?msg=supok&&id_client=" . $membre['id_client'] . "' onclick='return(confirm(\"Etes-vous sure de vouloir supprimer ce membre?\"));'> X </a></td>";
		if ($membre['statut'] == 0)
			echo "<td><a href='gestion_membre.php?msg=pass_admin&&id_client=" . $membre['id_client'] . "' onclick='return(confirm(\"Etes-vous sùr de vouloir supprimer ce membre?\"));'> Passer en admin </a></td>";
		else
			echo "<td><a href='gestion_membre.php?msg=supp_admin&&id_client=" . $membre['id_client'] . "' onclick='return(confirm(\"Etes-vous sùr de vouloir supprimer ce membre?\"));'> Supprimer admin </a></td>";
		echo '</tr>';
	}
	echo '</table>';