// Code for sampling shadows in a circle instead of a square.
// Coundn't notice noticible quality difference
// This code uses points on a circle, but I also tested points in the circle.
// When checking in circle the points were sampled unifomly using sqrt(radius);
//    float shadow = 0.0;
//        vec2 p[] = {
//        vec2(0.7189482986612965, 0.6950635538222582),
//        vec2(-0.26092490179162214, -0.9653591019019981),
//        vec2(0.9596155732669381, 0.2813146841947034),
//        vec2(0.9842020041751712, 0.17704918801727476),
//        vec2(-0.9989096387155044, -0.046685476127597374),
//        vec2(-0.9946289994840701, 0.10350436408827256),
//        vec2(0.5191009315782861, -0.8547129476230926),
//        vec2(-0.6773567132949135, -0.7356547308040045),
//        vec2(-0.6227655216433561, -0.7824085282333512),
//        vec2(0.9995762894732906, -0.02910741353690619),
//        vec2(-0.17582027747737933, 0.9844222823706182),
//        vec2(0.41417386624700864, -0.9101977853840367),
//        vec2(-0.9631875481080097, 0.2688303315656188),
//        vec2(-0.22227016737383506, -0.9749851140892395),
//        vec2(-0.9760538334281978, 0.21752911127046837),
//        vec2(0.6878416465242143, -0.7258607781846721),
//    };
//		Also try scaling the texelSize.
//    vec2 texelSize = 1.0 / vec2(textureSize(shadowMaps[layer], 0));
//    for (int i = 0; i < 16; i++)
//    {
//        float closestDepth = texture2D(shadowMaps[layer], posLightSpace.xy + p[i] * texelSize).r;
//        shadow += (currentDepth - bias) > closestDepth ? 1.0 : 0.0;        
//    }
//    shadow /= 16.0;