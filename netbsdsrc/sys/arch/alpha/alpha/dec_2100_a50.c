/* $NetBSD: dec_2100_a50.c,v 1.36 1997/10/17 19:00:12 mjacob Exp $ */

/*
 * Copyright (c) 1995, 1996, 1997 Carnegie-Mellon University.
 * All rights reserved.
 *
 * Author: Chris G. Demetriou
 * 
 * Permission to use, copy, modify and distribute this software and
 * its documentation is hereby granted, provided that both the copyright
 * notice and this permission notice appear in all copies of the
 * software, derivative works or modified versions, and any portions
 * thereof, and that both notices appear in supporting documentation.
 * 
 * CARNEGIE MELLON ALLOWS FREE USE OF THIS SOFTWARE IN ITS "AS IS" 
 * CONDITION.  CARNEGIE MELLON DISCLAIMS ANY LIABILITY OF ANY KIND 
 * FOR ANY DAMAGES WHATSOEVER RESULTING FROM THE USE OF THIS SOFTWARE.
 * 
 * Carnegie Mellon requests users of this software to return to
 *
 *  Software Distribution Coordinator  or  Software.Distribution@CS.CMU.EDU
 *  School of Computer Science
 *  Carnegie Mellon University
 *  Pittsburgh PA 15213-3890
 *
 * any improvements or extensions that they make and grant Carnegie the
 * rights to redistribute these changes.
 */
/*
 * Additional Copyright (c) 1997 by Matthew Jacob for NASA/Ames Research Center
 */
#include <sys/cdefs.h>			/* RCS ID & Copyright macro defns */

__KERNEL_RCSID(0, "$NetBSD: dec_2100_a50.c,v 1.36 1997/10/17 19:00:12 mjacob Exp $");

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/device.h>
#include <sys/termios.h>
#include <dev/cons.h>

#include <machine/rpb.h>
#include <machine/autoconf.h>
#include <machine/conf.h>
#include <machine/bus.h>

#include <dev/ic/comreg.h>
#include <dev/ic/comvar.h>

#include <dev/isa/isavar.h>
#include <dev/pci/pcireg.h>
#include <dev/pci/pcivar.h>

#include <alpha/pci/apecsreg.h>
#include <alpha/pci/apecsvar.h>

#include <dev/scsipi/scsi_all.h>
#include <dev/scsipi/scsipi_all.h>
#include <dev/scsipi/scsiconf.h>

#ifndef CONSPEED
#define CONSPEED TTYDEF_SPEED
#endif
static int comcnrate = CONSPEED;

void dec_2100_a50_init __P((void));
static void dec_2100_a50_cons_init __P((void));
static void dec_2100_a50_device_register __P((struct device *, void *));

void
dec_2100_a50_init()
{
	platform.family = "AlphaStation 200/400 (\"Avanti\")";
	switch (hwrpb->rpb_variation & SV_ST_MASK) {
	case SV_ST_AVANTI:
	case SV_ST_AVANTI_XXX:		/* XXX apparently the same? */
		platform.model = "AlphaStation 400 4/233 (\"Avanti\")";
		break;

	case SV_ST_MUSTANG2_4_166:
		platform.model = "AlphaStation 200 4/166 (\"Mustang II\")";
		break;

	case SV_ST_MUSTANG2_4_233:
		platform.model = "AlphaStation 200 4/233 (\"Mustang II\")";
		break;

	case SV_ST_AVANTI_4_266:
		platform.model = "AlphaStation 250 4/266";
		break;

	case SV_ST_MUSTANG2_4_100:
		platform.model = "AlphaStation 200 4/100 (\"Mustang II\")";
		break;

	case SV_ST_AVANTI_4_233:
		platform.model = "AlphaStation 255/233";
		break;

	default:
	{
		/* string is 24 bytes plus 64 bit hex number (16 byte) */
		static char s[42];
		sprintf(s, "unknown model variation %lx",
		    hwrpb->rpb_variation & SV_ST_MASK);
		platform.model = (const char *) s;
		break;
	}
	}
	platform.iobus = "apecs";
	platform.cons_init = dec_2100_a50_cons_init;
	platform.device_register = dec_2100_a50_device_register;
}

static void
dec_2100_a50_cons_init()
{
	struct ctb *ctb;
	struct apecs_config *acp;
	extern struct apecs_config apecs_configuration;

	acp = &apecs_configuration;
	apecs_init(acp, 0);

	ctb = (struct ctb *)(((caddr_t)hwrpb) + hwrpb->rpb_ctb_off);

	switch (ctb->ctb_term_type) {
	case 2: 
		/* serial console ... */
		/* XXX */
		{
			/*
			 * Delay to allow PROM putchars to complete.
			 * FIFO depth * character time,
			 * character time = (1000000 / (defaultrate / 10))
			 */
			DELAY(160000000 / comcnrate);

			if(comcnattach(&acp->ac_iot, 0x3f8, comcnrate,
			    COM_FREQ,
			    (TTYDEF_CFLAG & ~(CSIZE | PARENB)) | CS8))
				panic("can't init serial console");

			break;
		}

	case 3:
		/* display console ... */
		/* XXX */
		if ((ctb->ctb_turboslot & 0xffff) == 0)
			isa_display_console(&acp->ac_iot, &acp->ac_memt);
		else
			pci_display_console(&acp->ac_iot, &acp->ac_memt,
			    &acp->ac_pc, (ctb->ctb_turboslot >> 8) & 0xff,
			    ctb->ctb_turboslot & 0xff, 0);
		break;

	default:
		printf("ctb->ctb_term_type = 0x%lx\n", ctb->ctb_term_type);
		printf("ctb->ctb_turboslot = 0x%lx\n", ctb->ctb_turboslot);

		panic("consinit: unknown console type %d\n",
		    ctb->ctb_term_type);
	}
}

static void
dec_2100_a50_device_register(dev, aux)
	struct device *dev;
	void *aux;
{
	static int found, initted, scsiboot, netboot;
	static struct device *pcidev, *scsidev;
	struct bootdev_data *b = bootdev_data;
	struct device *parent = dev->dv_parent;
	struct cfdata *cf = dev->dv_cfdata;
	struct cfdriver *cd = cf->cf_driver;

	if (found)
		return;

	if (!initted) {
		scsiboot = (strcmp(b->protocol, "SCSI") == 0);
		netboot = (strcmp(b->protocol, "BOOTP") == 0);
#if 0
		printf("scsiboot = %d, netboot = %d\n", scsiboot, netboot);
#endif
		initted =1;
	}

	if (pcidev == NULL) {
		if (strcmp(cd->cd_name, "pci"))
			return;
		else {
			struct pcibus_attach_args *pba = aux;

			if ((b->slot / 1000) != pba->pba_bus)
				return;
	
			pcidev = dev;
#if 0
			printf("\npcidev = %s\n", pcidev->dv_xname);
#endif
			return;
		}
	}

	if (scsiboot && (scsidev == NULL)) {
		if (parent != pcidev)
			return;
		else {
			struct pci_attach_args *pa = aux;

			if ((b->slot % 1000) != pa->pa_device)
				return;

			/* XXX function? */
	
			scsidev = dev;
#if 0
			printf("\nscsidev = %s\n", scsidev->dv_xname);
#endif
			return;
		}
	}

	if (scsiboot &&
	    (!strcmp(cd->cd_name, "sd") ||
	     !strcmp(cd->cd_name, "st") ||
	     !strcmp(cd->cd_name, "cd"))) {
		struct scsipibus_attach_args *sa = aux;

		if (parent->dv_parent != scsidev)
			return;

		if (b->unit / 100 != sa->sa_sc_link->scsipi_scsi.target)
			return;

		/* XXX LUN! */

		switch (b->boot_dev_type) {
		case 0:
			if (strcmp(cd->cd_name, "sd") &&
			    strcmp(cd->cd_name, "cd"))
				return;
			break;
		case 1:
			if (strcmp(cd->cd_name, "st"))
				return;
			break;
		default:
			return;
		}

		/* we've found it! */
		booted_device = dev;
#if 0
		printf("\nbooted_device = %s\n", booted_device->dv_xname);
#endif
		found = 1;
	}

	if (netboot) {
		if (parent != pcidev)
			return;
		else {
			struct pci_attach_args *pa = aux;

			if ((b->slot % 1000) != pa->pa_device)
				return;

			/* XXX function? */
	
			booted_device = dev;
#if 0
			printf("\nbooted_device = %s\n", booted_device->dv_xname);
#endif
			found = 1;
			return;
		}
	}
}
