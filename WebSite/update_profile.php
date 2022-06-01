<?php
require_once("inc/init.inc.php");
if(!internauteEstConnecte())
{
	header("location:connexion.php");
	exit();
}
if($_POST)
{
	if(!empty($_POST['mdp']))
	{
		executeRequete("UPDATE client SET mdp='$_POST[mdp]', nom='$_POST[nom]', prenom='$_POST[prenom]', email='$_POST[email]', civilite='$_POST[civilite]', ville='$_POST[ville]', code_postal='$_POST[code_postal]', adresse='$_POST[adresse]' where id_client='".$_SESSION['client']['id_client']."'");
		unset($_SESSION['client']);		
		foreach($client as $indice => $element)
		{
			if($indice != 'mdp')
			{
				$_SESSION['client'][$indice] = $element;
			}
			else
			{
				$_SESSION['client'][$indice] = $_POST['mdp'];
			}
		}
		header("location:update_profile.php?action=modif");
	}
	else
	{
		$contenu .= '<div class="erreur">le nouveau mot de passe doit étre renseigné !</div>';
	}
}
if(isset($_GET['action']) && $_GET['action'] == 'modif')
{
	$contenu .= "la modification a bien été prise en compte";
}


require_once("inc/haut.inc.php");
echo $contenu;
?>
		<h2 class="forms-titles"> Modification de vos informations </h2>
		<?php
			echo "vous étes connecté sous le pseudo: " . $_SESSION['client']['pseudo'];
		?><br /><br />
		<form method="post" enctype="multipart/form-data" action="update_profile.php" class="forms">
		<input type="hidden" id="id_client" name="id_client" value="<?php if(isset($_SESSION['client'])) echo $_SESSION['client']['id_client']; ?>" />
			<label for="pseudo">Pseudo</label>
				<input disabled type="text" id="pseudo" name="pseudo" value="<?php if(isset($_SESSION['client'])) echo $_SESSION['client']['pseudo']; ?>"/><br />
				<input type="hidden" id="pseudo" name="pseudo" value="<?php if(isset($_SESSION['client'])) echo $_SESSION['client']['pseudo']; ?>"/>
			
			<label for="mdp">Nouv. Mot de passe</label>
				<input type="password" id="mdp" name="mdp" value="<?php if(isset($mdp)) echo $mdp; ?>"/><br /><br />
			
			<label for="nom">Nom</label>
				<input type="text" id="nom" name="nom" value="<?php if(isset($_SESSION['client'])) echo $_SESSION['client']['nom']; ?>"/><br />
			
			<label for="prenom">Prénom</label>
				<input type="text" id="prenom" name="prenom" value="<?php if(isset($_SESSION['client'])) echo $_SESSION['client']['prenom']; ?>"/><br />

			<label for="email">Email</label>
				<input type="text" id="email" name="email" value="<?php if(isset($_SESSION['client'])) echo $_SESSION['client']['email']; ?>"/><br />
			
			<label for="sexe">Sexe</label>
					<select id="civilite" name="civilite">
						<option value="m" <?php if(isset($_SESSION['client']['civilite']) && $_SESSION['client']['civilite'] == "m") echo "selected"; ?>>Homme</option>
						<option value="f" <?php if(isset($_SESSION['client']['civilite']) && $_SESSION['client']['civilite'] == "f") echo "selected"; ?>>Femme</option>
					</select><br />
					
			<label for="ville">Ville</label>
				<input type="text" id="ville" name="ville" value="<?php if(isset($_SESSION['client'])) echo $_SESSION['client']['ville']; ?>"/><br />
			
		<label for="code_postal">Cp</label>
			<input type="text" id="code_postal" name="code_postal" value="<?php if(isset($_SESSION['client'])) echo $_SESSION['client']['code_postal']; ?>"/><br />
			
		<label for="adresse">Adresse</label>
					<textarea id="adresse" name="adresse"><?php if(isset($_SESSION['client'])) echo $_SESSION['client']['adresse']; ?></textarea>
					<input type="hidden" name="statut" value="<?php if(isset($_SESSION['client'])) echo $_SESSION['client']['statut']; ?>"/><br />
			<br /><br />
			<input type="submit" class="submit" name="modification" value="modification"/>
	</form><br />
</div>