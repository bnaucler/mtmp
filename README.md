# mtmp 0.1
A cli tool for weather data.

## Written by
Björn Westerberg Nauclér (mail@bnaucler.se) 2017

Tested on Arch Linux 4.11 (x86\_64)

## Dependencies
mtmp requires libjansson, libcurl and libgeoip, and connects to [OpenWeathermap](http://openweathermap.org) for data retrieval.

You will also need to download the [GeoLite City database](http://geolite.maxmind.com/download/geoip/database/GeoLiteCity.dat.gz) from Maxmind, and unzip to `/var/db/GeoLiteCity.dat`

## Installation
`sudo make all install`

## Usage
`mtmp` - Gives current temperature based on geolocation  
`mtmp amsterdam` - Current temperature for Amsterdam

## Disclaimer
Wrote this to experiment with libcurl and libjansson; not to create a solid product.

Currently my personal (limited) API key to OpenWeathermap is hardcoded in the source. Please change to your own key if you intend to send more than an occasional request.

## License
MIT (do whatever you want)
