---
title: "Recharging a BMW key -- an engineer's approach"
date: 2021-11-13T17:09:16-07:00
draft: true
description: "How to recharge a dead BMW 328 (e90) key"
categories:
- Writing
tags:
- e90
- bmw
- electronics
---

![Opened key](/recharge_bmw_key/opened.jpg)

# BMW has strong engineers
... whose decisions are sometime questionable. Don't get me wrong, I love my e90, however things like plastic
valve cover gaskets, necessitating wheel removal for headlight replacement, and rear seats that lack an opening
to the trunk have me wondering: "_who the hell signed off on this in Munich?_"


The latest gripe I have with BMW is about the key design. Since COVID was in full swing over the last two years,
I haven't really been going anywhere, so the car has been sitting idle -- good for the environment, bad for the car.
One day that I finally had somewhere to be, I walked up to the car and was unpleasantly surprised to find out that
it wouldn't open as the key battery had died.

# Where it gets annoying
Dying is as inherent to batteries as it is to you and me; it's bound to happen at some point. You know this, I know this,
and the Germans know this. And yet, this is their solution:

> Use each remote control at least twice a year for longer road trips in order to maintain the batteries' charge status

It's a little late for that, chief. The problem is that the car won't even start if the key is discharged, which means 
there is no supported way to get the charge back. This is really a missed opportunity, which has simple solutions:

* Expose metal contacts as a backup way to charge
* Provide charge if a key is inserted but not communicating
* Use a replaceable battery

I'm sure there are others.

# Where it gets really annoying
The batteries use a simple [inductive charging](https://en.wikipedia.org/wiki/Inductive_charging) mechanism: you apply a
signal at a set frequency in one coil (car-side) and when the key is inserted, there is a current induced in the coil in
in the key. Boom, you're charging. The issue is that the car-side coil isn't providing current, and its specifications
are not easily available.

Well, the dealer should be able to help with this, right? Wrong. Not only do they not have a bench-top charger for the 
keys, but they also want to charge $300 for a new key comprising a battery, 2 injection-molded pieces of
plastic, and a 2-layer PCBA that any hobbyist can assemble using a hot-plate and some tweezers. Unbelievable.

Okay, enough complaining. Let's get to the meat and potatoes.

# How to recharge the key battery
Charging the battery inside the key is ultimately a simple process: open the key, apply a current to the terminals,
and close it back up. That's it. You could also just replace the battery, but that's less interesting and wasteful.

### Tools
![Required tools](/recharge_bmw_key/tools.jpg)
* Utility knife for opening the key
* Super glue for closing the key back up
* A soldering iron for attaching wires to battery terminals
* Thin wires for connecting to the battery
* A resistor (value dependent on approach)
* (not shown) Charging apparatus

### Opening the enclosure
It makes me sad knowing that my original key will never be as weatherproof or as resilient to damage as
it one was, but then I remember: the key is effectively garbage at this point, so this is the only way to
give it any value again.

First, remove the metal glove-box key from its slot. 
Take the knife and firmly cut along the seam on the outside of the key fob. Some parts go in really easy, some don't.
Be patient -- especially around the keyring area -- as the knife can slip and cut you. Also be sure not to insert it
more than a few millimeters to avoid damaging any internal circuitry.

Once it feels pretty open, use the knife as a wedge, and carefully pry apart the two halves.
![Opening the enclosure](/recharge_bmw_key/opening.jpg)

### Quick detour into battery chemistry
https://industrial.panasonic.com/cdbs/www-data/pdf2/AAF4000/AAF4000C4.pdf
https://industrial.panasonic.com/cdbs/www-data/pdf/AAF4000/AAF4000COL6.pdf
