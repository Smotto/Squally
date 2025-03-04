#pragma once

#include "Critters/Gull.h"
#include "Critters/Horse.h"
#include "Critters/Reindeer.h"
#include "Critters/Lion.h"
#include "Critters/Seagull.h"
#include "Enemies/BallmerPeaks/Cryogen.h"
#include "Enemies/BallmerPeaks/FrostFiend.h"
#include "Enemies/BallmerPeaks/GoblinElf.h"
#include "Enemies/BallmerPeaks/IceGolem.h"
#include "Enemies/BallmerPeaks/PenguinGrunt.h"
#include "Enemies/BallmerPeaks/PenguinWarrior.h"
#include "Enemies/BallmerPeaks/Santa.h"
#include "Enemies/BallmerPeaks/SnowFiend.h"
#include "Enemies/BallmerPeaks/ToySoldierGoblin.h"
#include "Enemies/BallmerPeaks/Viking.h"
#include "Enemies/BallmerPeaks/WaterElemental.h"
#include "Enemies/BallmerPeaks/Yeti.h"
#include "Enemies/CastleValgrind/Abomination.h"
#include "Enemies/CastleValgrind/Agnes.h"
#include "Enemies/CastleValgrind/Barbarian.h"
#include "Enemies/CastleValgrind/Jack.h"
#include "Enemies/CastleValgrind/ReanimatedFighter.h"
#include "Enemies/CastleValgrind/Reaper.h"
#include "Enemies/CastleValgrind/SkeletalBaron.h"
#include "Enemies/CastleValgrind/Thug.h"
#include "Enemies/CastleValgrind/VampireLord.h"
#include "Enemies/CastleValgrind/Vampiress.h"
#include "Enemies/CastleValgrind/Werewolf.h"
#include "Enemies/CastleValgrind/Wraith.h"
#include "Enemies/FirewallFissure/Asmodeus.h"
#include "Enemies/FirewallFissure/DemonArcher.h"
#include "Enemies/FirewallFissure/DemonDragon.h"
#include "Enemies/FirewallFissure/DemonGhost.h"
#include "Enemies/FirewallFissure/DemonGrunt.h"
#include "Enemies/FirewallFissure/DemonRogue.h"
#include "Enemies/FirewallFissure/DemonShaman.h"
#include "Enemies/FirewallFissure/DemonSwordsman.h"
#include "Enemies/FirewallFissure/DemonWarrior.h"
#include "Enemies/FirewallFissure/FireElemental.h"
#include "Enemies/FirewallFissure/FireTiger.h"
#include "Enemies/FirewallFissure/LavaGolem.h"
#include "Enemies/DataMines/EarthElemental.h"
#include "Enemies/DataMines/EarthGolem.h"
#include "Enemies/DataMines/ForestGolem.h"
#include "Enemies/DataMines/Krampus.h"
#include "Enemies/DataMines/Rhinoman.h"
#include "Enemies/DataMines/Shaman.h"
#include "Enemies/DataMines/SkeletalArcher.h"
#include "Enemies/DataMines/SkeletalNecromancer.h"
#include "Enemies/DataMines/SkeletalPirate.h"
#include "Enemies/DataMines/SkeletalWarrior.h"
#include "Enemies/DataMines/TikiGolem.h"
#include "Enemies/Deprecated/Gargoyle.h"
#include "Enemies/Deprecated/Gorilla.h"
#include "Enemies/Deprecated/LightningGolem.h"
#include "Enemies/Deprecated/Scarecrow.h"
#include "Enemies/Deprecated/Shade.h"
#include "Enemies/EndianForest/Centaur.h"
#include "Enemies/EndianForest/Cyclops.h"
#include "Enemies/EndianForest/GoblinGruntBoar.h"
#include "Enemies/EndianForest/GoblinShaman.h"
#include "Enemies/EndianForest/GoblinWarriorPig.h"
#include "Enemies/EndianForest/Gorgon.h"
#include "Enemies/EndianForest/KingGrogg.h"
#include "Enemies/EndianForest/Ogre.h"
#include "Enemies/EndianForest/OrcBomber.h"
#include "Enemies/EndianForest/OrcGrunt.h"
#include "Enemies/EndianForest/OrcSwordsman.h"
#include "Enemies/EndianForest/OrcWarrior.h"
#include "Enemies/EndianForest/Orthrus.h"
#include "Enemies/EndianForest/Troll.h"
#include "Enemies/LambdaCrypts/Assassin.h"
#include "Enemies/LambdaCrypts/BoneFiend.h"
#include "Enemies/LambdaCrypts/BoneKnight.h"
#include "Enemies/LambdaCrypts/Hunter.h"
#include "Enemies/LambdaCrypts/KingZul.h"
#include "Enemies/LambdaCrypts/Lazarus.h"
#include "Enemies/LambdaCrypts/Mystic.h"
#include "Enemies/LambdaCrypts/ReanimatedPig.h"
#include "Enemies/LambdaCrypts/SkeletalCleaver.h"
#include "Enemies/LambdaCrypts/SkeletalKnight.h"
#include "Enemies/LambdaCrypts/SkeletalPriestess.h"
#include "Enemies/LambdaCrypts/Undead.h"
#include "Enemies/LambdaCrypts/Warlock.h"
#include "Enemies/LambdaCrypts/Zombie.h"
#include "Enemies/LambdaCrypts/ZombieElric.h"
#include "Enemies/TrainingDummy.h"
#include "Enemies/UnderflowRuins/AnubisPup.h"
#include "Enemies/UnderflowRuins/AnubisWarrior.h"
#include "Enemies/UnderflowRuins/DarkTiger.h"
#include "Enemies/UnderflowRuins/LionMan.h"
#include "Enemies/UnderflowRuins/Lioness.h"
#include "Enemies/UnderflowRuins/Medusa.h"
#include "Enemies/UnderflowRuins/Mermaid.h"
#include "Enemies/UnderflowRuins/Minotaur.h"
#include "Enemies/UnderflowRuins/MummyPriest.h"
#include "Enemies/UnderflowRuins/MummyWarrior.h"
#include "Enemies/UnderflowRuins/Osiris.h"
#include "Enemies/UnderflowRuins/TigerMan.h"
#include "Enemies/UnderflowRuins/Tigress.h"
#include "Enemies/VoidStar/Perceptron.h"
#include "Enemies/VoidStar/Exterminator.h"
#include "Enemies/VoidStar/VoidDemon.h"
#include "EntityPreview.h"
#include "Helpers/BallmerPeaks/Snowman.h"
#include "Helpers/BallmerPeaks/YetiBaby.h"
#include "Helpers/CastleValgrind/Grim.h"
#include "Helpers/CastleValgrind/Knight.h"
#include "Helpers/FirewallFissure/CritterDemon.h"
#include "Helpers/DataMines/Gecky.h"
#include "Helpers/EndianForest/Guano.h"
#include "Helpers/EndianForest/Scrappy.h"
#include "Helpers/LambdaCrypts/Ghost.h"
#include "Helpers/UnderflowRuins/GuanoPetrified.h"
#include "Helpers/VoidStar/Robot.h"
#include "Npcs/BallmerPeaks/Aspen.h"
#include "Npcs/BallmerPeaks/Bodom.h"
#include "Npcs/BallmerPeaks/Cookie.h"
#include "Npcs/BallmerPeaks/Cooper.h"
#include "Npcs/BallmerPeaks/Jingles.h"
#include "Npcs/BallmerPeaks/Juniper.h"
#include "Npcs/BallmerPeaks/Kringle.h"
#include "Npcs/BallmerPeaks/Luna.h"
#include "Npcs/BallmerPeaks/Nessie.h"
#include "Npcs/BallmerPeaks/PrincessPepper.h"
#include "Npcs/BallmerPeaks/Theldar.h"
#include "Npcs/BallmerPeaks/Tinsel.h"
#include "Npcs/BallmerPeaks/Ysara.h"
#include "Npcs/CastleValgrind/Atreus.h"
#include "Npcs/CastleValgrind/Gaunt.h"
#include "Npcs/CastleValgrind/Illia.h"
#include "Npcs/CastleValgrind/KingRedsong.h"
#include "Npcs/CastleValgrind/KingRedsongSlime.h"
#include "Npcs/CastleValgrind/Leon.h"
#include "Npcs/CastleValgrind/Leopold.h"
#include "Npcs/CastleValgrind/Leroy.h"
#include "Npcs/CastleValgrind/PrincessOpal.h"
#include "Npcs/CastleValgrind/Raven.h"
#include "Npcs/CastleValgrind/Rogas.h"
#include "Npcs/CastleValgrind/Thurstan.h"
#include "Npcs/CastleValgrind/Tyracius.h"
#include "Npcs/CastleValgrind/Zana.h"
#include "Npcs/FirewallFissure/Ash.h"
#include "Npcs/FirewallFissure/Brine.h"
#include "Npcs/FirewallFissure/Celeste.h"
#include "Npcs/FirewallFissure/Cindra.h"
#include "Npcs/FirewallFissure/Hades.h"
#include "Npcs/FirewallFissure/Lucifer.h"
#include "Npcs/FirewallFissure/Magnus.h"
#include "Npcs/FirewallFissure/Pan.h"
#include "Npcs/FirewallFissure/Mittens.h"
#include "Npcs/FirewallFissure/Ragnis.h"
#include "Npcs/FirewallFissure/Scaldor.h"
#include "Npcs/FirewallFissure/Thatcher.h"
#include "Npcs/FirewallFissure/QueenElise.h"
#include "Npcs/DataMines/Bancroft.h"
#include "Npcs/DataMines/Bonnie.h"
#include "Npcs/DataMines/Brock.h"
#include "Npcs/DataMines/Burch.h"
#include "Npcs/DataMines/Godiva.h"
#include "Npcs/DataMines/Mildred.h"
#include "Npcs/DataMines/Olive.h"
#include "Npcs/DataMines/PrincessDawn.h"
#include "Npcs/DataMines/Raka.h"
#include "Npcs/DataMines/Ralston.h"
#include "Npcs/DataMines/Rusty.h"
#include "Npcs/DataMines/Shen.h"
#include "Npcs/EndianForest/Ara.h"
#include "Npcs/EndianForest/Bard.h"
#include "Npcs/EndianForest/Chiron.h"
#include "Npcs/EndianForest/Elriel.h"
#include "Npcs/EndianForest/Gramps.h"
#include "Npcs/EndianForest/Lycan.h"
#include "Npcs/EndianForest/Mara.h"
#include "Npcs/EndianForest/Matu.h"
#include "Npcs/EndianForest/Minos.h"
#include "Npcs/EndianForest/Polyphemus.h"
#include "Npcs/EndianForest/QueenLiana.h"
#include "Npcs/EndianForest/Robin.h"
#include "Npcs/EndianForest/Toben.h"
#include "Npcs/LambdaCrypts/Amelia.h"
#include "Npcs/LambdaCrypts/Azmus.h"
#include "Npcs/LambdaCrypts/Elric.h"
#include "Npcs/LambdaCrypts/Garrick.h"
#include "Npcs/LambdaCrypts/Johann.h"
#include "Npcs/LambdaCrypts/PrincessNebea.h"
#include "Npcs/LambdaCrypts/Roger.h"
#include "Npcs/LambdaCrypts/Thion.h"
#include "Npcs/LambdaCrypts/Ursula.h"
#include "Npcs/LambdaCrypts/Vesuvius.h"
#include "Npcs/LambdaCrypts/Viper.h"
#include "Npcs/LambdaCrypts/Zelina.h"
#include "Npcs/Mages/Alder.h"
#include "Npcs/Mages/Aster.h"
#include "Npcs/Mages/Cypress.h"
#include "Npcs/Mages/Fraya.h"
#include "Npcs/Mages/Igneus.h"
#include "Npcs/Mages/Jasper.h"
#include "Npcs/Mages/Mabel.h"
#include "Npcs/Mages/Marcel.h"
#include "Npcs/Mages/Merlin.h"
#include "Npcs/Mages/Necron.h"
#include "Npcs/Mages/Piper.h"
#include "Npcs/Mages/Sarude.h"
#include "Npcs/Transition/Blackbeard.h"
#include "Npcs/Transition/Drak.h"
#include "Npcs/Transition/Dudly.h"
#include "Npcs/Transition/Finch.h"
#include "Npcs/Transition/Garin.h"
#include "Npcs/Transition/Irmik.h"
#include "Npcs/UnderflowRuins/Ajax.h"
#include "Npcs/UnderflowRuins/Angel.h"
#include "Npcs/UnderflowRuins/Aphrodite.h"
#include "Npcs/UnderflowRuins/Appolo.h"
#include "Npcs/UnderflowRuins/Ares.h"
#include "Npcs/UnderflowRuins/Athena.h"
#include "Npcs/UnderflowRuins/Cleopatra.h"
#include "Npcs/UnderflowRuins/Geryon.h"
#include "Npcs/UnderflowRuins/Griffin.h"
#include "Npcs/UnderflowRuins/Hera.h"
#include "Npcs/UnderflowRuins/Horus.h"
#include "Npcs/UnderflowRuins/Poseidon.h"
#include "Npcs/UnderflowRuins/Rupert.h"
#include "Npcs/UnderflowRuins/Thor.h"
#include "Npcs/UnderflowRuins/Zeus.h"
#include "Npcs/VoidStar/Boron.h"
#include "Npcs/VoidStar/Radon.h"
#include "Npcs/VoidStar/Xenon.h"
#include "Special/KillingMachines/KillingMachine1.h"
#include "Special/KillingMachines/KillingMachine2.h"
#include "Special/KillingMachines/KillingMachine3.h"
#include "Special/KillingMachines/KillingMachine4.h"
#include "Special/KillingMachines/KillingMachine5.h"
#include "Special/KillingMachines/KillingMachine6.h"
#include "Special/Shopkeeper.h"
#include "Squally/Squally.h"
