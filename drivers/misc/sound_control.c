/*
 * Copyright 2013 Francisco Franco
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/init.h>
#include <linux/device.h>
#include <linux/miscdevice.h>

#define SOUNDCONTROL_VERSION 4

extern void update_headphones_volume_boost(unsigned int vol_boost);

extern void update_headset_volume_boost(int gain_boost);

extern void update_mic_gain(int gain_boost);

extern void update_speakers_volume_boost(int vol_boost);

/*
 * Volume boost value
 */

int speakers_boost = 0;
int speakers_limit = 10;
int speakers_limit_min = -10;

int boost = 0;
int boost_limit = 20;
int boost_limit_min = -20;

int headset_boost = 0;
int headset_boost_limit = 30;
int headset_boost_limit_min = -30;

int mic_gain = 0;
int mic_gain_limit = 10;

/*
 * Sysfs get/set entries
 */


static ssize_t speakers_boost_show(struct device *dev, struct device_attribute *attr, char *buf)
{
        return sprintf(buf, "%d\n", speakers_boost);

}

static ssize_t speakers_boost_store(struct device * dev, struct device_attribute * attr, const char * buf, size_t size)
{

        int new_val;
        sscanf(buf, "%d", &new_val);

        if (new_val != speakers_boost) {
                if (new_val <= speakers_limit_min)
			new_val = speakers_limit_min;

		else if (new_val >= speakers_limit)
			new_val = speakers_limit;

		pr_info("New speaker_boost: %d\n", new_val);

		speakers_boost = new_val;
		update_speakers_volume_boost(speakers_boost);
        }
	return size;
}

static ssize_t volume_boost_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    return sprintf(buf, "%d\n", boost);
}

static ssize_t volume_boost_store(struct device * dev, struct device_attribute * attr, const char * buf, size_t size)
{
    int new_val;

	sscanf(buf, "%d", &new_val);

	if (new_val != boost) {
		if (new_val <= boost_limit_min)
			new_val = boost_limit_min;

		else if (new_val >= boost_limit)
			new_val = boost_limit;

		pr_info("New volume_boost: %d\n", new_val);

		boost = new_val;
		update_headphones_volume_boost(boost);
	}

    return size;
}

static ssize_t headset_boost_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    return sprintf(buf, "%d\n", headset_boost);
}

static ssize_t headset_boost_store(struct device * dev, struct device_attribute * attr, const char * buf, size_t size)
{
    int new_val;

	sscanf(buf, "%d", &new_val);

	if (new_val != headset_boost) {
		if (new_val <= headset_boost_limit_min)
			new_val = headset_boost_limit_min;

		else if (new_val >= headset_boost_limit)
			new_val = headset_boost_limit;

		pr_info("New headset_boost: %d\n", new_val);

		headset_boost = new_val;
		update_headset_volume_boost(headset_boost);
	}

    return size;
}

static ssize_t mic_gain_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    return sprintf(buf, "%d\n", mic_gain);
}

static ssize_t mic_gain_store(struct device * dev, struct device_attribute * attr, const char * buf, size_t size)
{
    int new_val;

	sscanf(buf, "%d", &new_val);

	if (new_val != mic_gain) {
		if (new_val >= mic_gain_limit)
			new_val = mic_gain_limit;

		pr_info("New Mic gain_boost: %d\n", new_val);

		mic_gain = new_val;
		update_headset_volume_boost(mic_gain);
	}

    return size;
}

static ssize_t soundcontrol_version(struct device * dev, struct device_attribute * attr, char * buf)
{
    return sprintf(buf, "%d\n", SOUNDCONTROL_VERSION);
}

static DEVICE_ATTR(volume_boost, 0664, volume_boost_show, volume_boost_store);
static DEVICE_ATTR(headset_boost, 0664, headset_boost_show, headset_boost_store);
static DEVICE_ATTR(mic_gain, 0664, mic_gain_show, mic_gain_store);
static DEVICE_ATTR(speakers_boost, 0664, speakers_boost_show, speakers_boost_store);

static DEVICE_ATTR(version, 0664 , soundcontrol_version, NULL);

static struct attribute *soundcontrol_attributes[] = 
{
	&dev_attr_volume_boost.attr,
	&dev_attr_headset_boost.attr,
	&dev_attr_mic_gain.attr,
        &dev_attr_speakers_boost.attr,
	&dev_attr_version.attr,
	NULL
};

static struct attribute_group soundcontrol_group = 
{
	.attrs  = soundcontrol_attributes,
};

static struct miscdevice soundcontrol_device = 
{
	.minor = MISC_DYNAMIC_MINOR,
	.name = "soundcontrol",
};

static int __init soundcontrol_init(void)
{
    int ret;

    pr_info("%s misc_register(%s)\n", __FUNCTION__, soundcontrol_device.name);

    ret = misc_register(&soundcontrol_device);

    if (ret) {
	    pr_err("%s misc_register(%s) fail\n", __FUNCTION__, soundcontrol_device.name);
	    return 1;
	}

    if (sysfs_create_group(&soundcontrol_device.this_device->kobj, &soundcontrol_group) < 0) {
	    pr_err("%s sysfs_create_group fail\n", __FUNCTION__);
	    pr_err("Failed to create sysfs group for device (%s)!\n", soundcontrol_device.name);
	}

    return 0;
}
late_initcall(soundcontrol_init);
