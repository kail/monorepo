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
- diy
---

![Opened key](/recharge_bmw_key/opened.jpg)

## BMW has strong engineers
... whose decisions are sometime questionable. Don't get me wrong, I love my e90, however things like plastic
valve cover gaskets, necessitating wheel removal for headlight replacement, and rear seats that lack an opening
to the trunk have me wondering: "_who the hell signed off on this in Munich?_"


The latest gripe I have with BMW is about the key design. Since COVID was in full swing over the last two years,
I haven't really been going anywhere, so the car has been sitting idle -- good for the environment, bad for the car.
One day that I finally had somewhere to be, I walked up to the car and was unpleasantly surprised to find out that
it wouldn't open as the key battery had died.

## Where it gets annoying
Dying is as inherent to batteries as it is to you and me; it's bound to happen at some point. You know this, I know this,
and the Germans know this. And yet, this is their solution:

> Use each remote control at least twice a year for longer road trips in order to maintain the batteries' charge status

It's a little late for that, chief. The problem is that the car won't even start if the key is discharged, which means 
there is no supported way to get the charge back. This is really a missed opportunity, which has simple solutions:

* Expose metal contacts as a backup way to charge
* Provide charge if a key is inserted but not communicating
* Use a replaceable battery

I'm sure there are others.

## Where it gets _really_ annoying
The batteries use a simple [inductive charging](https://en.wikipedia.org/wiki/Inductive_charging) mechanism: you apply a
signal at a set frequency in one coil (car-side) and when the key is inserted, there is a current induced in the coil in
in the key. Boom, you're charging. The issue is that the car-side coil isn't providing current, and its specifications
are not easily available.

Well, the dealer should be able to help with this, right? Wrong. Not only do they not have a bench-top charger for the 
keys, but they also want to charge $300 for a new key comprising a battery, 2 injection-molded pieces of
plastic, and a 2-layer PCBA that any hobbyist can assemble using a hot-plate and some tweezers. Unbelievable.

Okay, enough complaining. Let's get to the meat and potatoes.

## How to recharge the key battery
Charging the battery inside the key is ultimately a simple process: open the key, apply a current to the terminals,
and close it back up. That's it. You could also just replace the battery, but that's less interesting and wasteful.
Before getting started, though, you need to decide _how_ you will be charging the battery. Note that if you're just
planning on replacing them, you want a [H-Type VL2020](https://www.amazon.com/OEM-VL2020-ML2020-Rechargeable-Battery/dp/B07X27S1BK/ref=psdc_10112775011_t1_B07W95J2L3)
and you can skip down to the [tools section](#tools).

### Quick detour into battery charging
The batteries used in these keys are [VL2020](https://industrial.panasonic.com/ww/products/pt/coin_rechargeable_lithium/models/VL2020) coin-type 
Lithium Vanadium rechargeable batteries. Generally when dealing with Lithium batteries, it's expected
that a charge controller is used, such as the [MCP73831](https://ww1.microchip.com/downloads/en/DeviceDoc/MCP73831-Family-Data-Sheet-DS20001984H.pdf), however
these batteries call for a much simpler circuit, according to the
[charging application note](/recharge_bmw_key/panasonic_vl.pdf):

![VL2020 charging circuit](/recharge_bmw_key/charge.png)

What this implies is that rather than the standard [Constant Current/Constant Voltage (CCCV)](https://en.wikipedia.org/wiki/Lithium-ion_battery#Charging_and_discharging) charge algorithm, this battery
operates in the constant voltage phase of the curve, due to the constant `3.4V` output from the regulator 
(not shown in image). The charging application note also specifies a maximum 
`1.5mA` charge current for the VL2020, so now we have our two limits: `3.4V` and `1.5mA` -- these must NEVER be exceeded.

### Tools
![Required tools](/recharge_bmw_key/tools.jpg)
* Utility knife for opening the key
* Super glue for closing the key back up
* A soldering iron for attaching wires to battery terminals
* Thin wires for connecting to the battery
* A resistor (value dependent on approach)
* Charging apparatus (not shown)

### Opening the enclosure
First, remove the metal glove-box key from its slot. 
Take the knife and firmly cut along the seam on the outside of the key fob. Some parts go in really easy, some don't.
Be patient -- especially around the keyring area -- as the knife can slip and cut you. Also be sure not to insert it
more than a few millimeters to avoid damaging any internal circuitry.

Once it feels pretty open, use the knife as a wedge, and carefully pry apart the two halves.

It makes me sad knowing that my original key will never be as weatherproof or as resilient to damage as
it one was, but then I remember: the key is effectively garbage at this point, so this is the only way to
give it any value again. Onwards.

![Opening the enclosure](/recharge_bmw_key/opening.jpg)

### Soldering charging wires
The next step is to attach wires to the battery terminals. The positive terminal is the one closest to the base
of the remote, and the negative is, well, the other one. Depending on how you will be charging (see below), you will also 
want to solder the resistor -- and optionally a diode -- to the other end of the positive lead, at this point.

![Attach charging cables](/recharge_bmw_key/attach.jpg)

### Charging with a lab power supply
Fortunately for me, I have a precision, desktop power supply called the [Otii Arc](https://www.qoitech.com/otii/) that
is the perfect tool for the job. Most power supplies _will not work_ for this approach, because they lack the
precision required.

If you have a precision supply, you do not need a current-limiting resistor, as you can just configure that limit
on the power supply itself; you will see that the output voltage is limited to
something below 3.4V, since there is no resistor to limit the current.

In order to charge, what you do is:
1. Set the voltage output to 3.4V
1. Set the current output to 1mA
1. Connect the charger to the wires, set, and forget

### Charging with a voltage source
Okay, so let's assume you don't have a power supply, what do you do? Well, you make yourself a voltage source! This
can be anything from another battery, to a cheap [USB supply](https://www.amazon.com/Step-Down-Module-CE009-Supply-Converter/dp/B07XTCX6G2), to any other lab power supply such
as [this one](https://www.amazon.com/Siglent-Technologies-SPD3303X-Triple-Output/dp/B01410O424) (note that I am in
no way advocating for this specific power supply).

The steps are similar to the above:
1. Solder a resistor (and potentially diode) to the positive lead
1. Set the lab supply to 3.4V or connect the fixed output voltage source
1. Connect the power supply to the resistor and negative terminal
1. Watch carefully for about an hour to ensure you don't burn your house down

### Charging with a voltage source


https://industrial.panasonic.com/cdbs/www-data/pdf2/AAF4000/AAF4000C4.pdf
https://industrial.panasonic.com/cdbs/www-data/pdf/AAF4000/AAF4000COL6.pdf
