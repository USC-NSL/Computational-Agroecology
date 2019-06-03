// Using google earth engine on MODIS/006/MOD11A1
// javascript 

var ee = require('@google/earthengine');
// REQUIRES PRIVATE KEY IMPLEMENTATION HERE

// SOME OF THE CODE BELOW FROM STACK OVERFLOW POST:
// https://gis.stackexchange.com/questions/307548/getting-temperature-data-of-given-point-using-modis-lst-data


// reduce the surface temperature image collection to a single image with stacked bands
function collToBands(imageCollection, bandName) {
  // stack all the bands to one single image
  // change the name of the bandname to the date it is acquired
  var changedNames = imageCollection.map( function(img){ 
    var dateString = ee.Date(img.get('system:time_start')).format('yyyy-MM-dd');
    return img.select(bandName).rename(dateString);
  });
  
  // Apply the function toBands() on the image collection to set all bands into one image
  var multiband = changedNames.toBands();
  // Reset the bandnames
  var names = multiband.bandNames();
  // rename the bandnames 
  var newNames = names.map(function(name){
    var ind = names.indexOf(name);
    return ee.String(names.get(ind)).slice(5);
  });
  return multiband.rename(newNames);
}

// MAIN FUNCTION TO GET AVERAGE DAILY TEMPERATURE
function getDailyTemp(start_date, end_date, latitude, longitude)
{
  var AOI1 = ee.Geometry.Polygon(
    [[latitude, longitude], [latitude+0.1,longitude+0.1], 
    [latitude+0.1,longitude], [latitude,longitude+0.1]]);
  // load the MODIS dataset (one image per day)
  var dataset1 = ee.ImageCollection('MODIS/006/MOD11A1')
          .filter(ee.Filter.date(start_date, end_date));
  // select the land surface temperature and add a pallete for it
  var landSurfaceTemperature = dataset1.select('LST_Day_1km');
  var point = ee.Geometry.Point([latitude, longitude]);
  
  var tempToDegrees = landSurfaceTemperature.map(function(image){
    return image.multiply(0.02).subtract(273.15).copyProperties(image,['system:time_start','system:time_end', 'system:index']);
  });

  // apply the function and print to console
  var multiband1 = collToBands(tempToDegrees, "LST_Day_1km");
  
  // get the temperature at a given point on the map for the given time spand and print to the console
  var temperature1 = multiband1.reduceRegion({reducer: ee.Reducer.mean(), geometry: point, scale: 1000, bestEffort: true});
  print('temperature at location: '+latitude +', '+longitude, temperature1);

}
getDailyTemp('2017-01-01', '2018-03-01', 2.3622940161999395, 42.569280018996714);

// end