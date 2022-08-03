#version 430 core

uniform vec3 directionalLightDir;

uniform sampler2D shadowMaps[16];
uniform mat4 worldToShadowMap[16];
uniform float cascadeZ[16];
uniform vec3 cascadeScale[16];
uniform int cascadeCount;
uniform mat4 worldToView;
uniform float farPlane;

uniform bool normalBias;
uniform float normalBiasScale;
uniform float constantBias;
uniform bool slopeBias;
uniform float slopeBiasScale;

uniform float scaleScale;

uniform bool doFiltering;
uniform float filteringRadius;

uniform bool useScaleAabb;

float shadowOld(vec3 posWorldSpace, vec3 normal)
{
    vec4 fragPosViewSpace = worldToView * vec4(posWorldSpace, 1.0);
    // This value should be negated or not based on the sign of the forward vector.
    // Just using abs makes it simpler becuase it works with any forward direction.
    float distanceFromView = abs(fragPosViewSpace.z);

    int layer = -1;
    for (int i = 0; i < cascadeCount; ++i)
    {
        if (distanceFromView < cascadeZ[i])
        {
            layer = i;
            break;
        }
    }
    if (layer == -1)
    {
        layer = cascadeCount;
    }

    float scale;

    float biasModifier = 0.5;
    if (useScaleAabb)
    {
       scale = cascadeScale[layer].z;
    }
    else
    {
        if (layer == cascadeCount)
        {
            scale = 1 / (farPlane * biasModifier);
        }
        else
        {
            scale = 1 / (cascadeZ[layer] * biasModifier);
        }
    }
    scale = 1.0f / scale;
    scale *= scaleScale;


    float d = dot(normal, directionalLightDir);
    float a = sqrt(1 - d * d);
    if (normalBias)
    {
        posWorldSpace += normal * a * normalBiasScale;
    }

    // Perspective divide isn't needed becuase orthographic projection is used.
    vec3 posLightSpace = (worldToShadowMap[layer] * vec4(posWorldSpace, 1.0)).xyz;
    // Transform from NDC to texture range. <[-1, -1], [1, 1]> to <[0, 0], [1, 1]>.
    posLightSpace = posLightSpace * 0.5 + 0.5;

    float currentDepth = posLightSpace.z;

    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if (currentDepth > 1.0)
    {
        return 0.0;
    }


    float bias = constantBias;
    // Because of precision issues shapes can shadow cast shadows on themselves. This effect is called shadow acne.
    // It can be mitigated by applying a bias to move the value closer than it actually is.
    // If the bias is too big an effect called petter panning happens which makes parts of the shadows disappear.
    // With this constant value this can be visible when looking close up on the edges of objects with shadows on them.
    // The most common techniques add a depth bias calculated based on slope or add the normal to the position of the point.
    // I couldn't get good result with neither of these methods. I decided to just use a constant bias varied based on the 
    // cascade though this way creates visible transitions between cascaded(further cascaded have more acne).
    // Should also probably scale the bias based on the shadow map resolution.

//     if (useScaleAabb)
//     {
//        bias *= cascadeScale[layer].z;
//     }
//     else
//     {
//         if (layer == cascadeCount)
//         {
//             bias *= 1 / (farPlane * biasModifier);
//         }
//         else
//         {
//             bias *= 1 / (cascadeZ[layer] * biasModifier);
//         }
//     }


    //float closestDepth = texture2D(shadowMaps[layer], posLightSpace.xy).r;
    //return (currentDepth - bias) > closestDepth ? 1.0 : 0.0;        

//    for (let i = 0 ; i < 81; i++)
//{
//  const angle = Math.random() * 2 * Math.PI;
//  const radius = Math.sqrt(Math.random()) * 5;
//  console.log(`vec2(${Math.sin(angle) * radius}, ${Math.cos(angle) * radius}),`)
//}

    vec2 samples[] = {
vec2(-0.9807698146299257, 3.13771918060967),
vec2(-3.7593829496620748, -0.6011227929995799),
vec2(2.263746874387353, 2.813703869529519),
vec2(-0.07386088891488883, 1.6686237210665213),
vec2(-3.4340075881475824, -3.0544529503366906),
vec2(3.6658549603624353, -1.1815130963435525),
vec2(0.751675248090715, -1.0104349165040476),
vec2(-1.4037455862230914, 1.3226949675256072),
vec2(1.5451384082974335, 0.5453050972756344),
vec2(1.4556014253933187, -1.3056838582605976),
vec2(2.7109985845547535, -3.093037807953159),
vec2(0.702893224701851, -2.0292067224200827),
vec2(-4.100462489447312, -1.1714061455673943),
vec2(3.241102706241288, 2.336709964639262),
vec2(1.8716801817642352, -2.9876120004239364),
vec2(-1.6816910054801781, 1.039266190244359),
vec2(2.816400543160108, -3.08178162801233),
vec2(0.7746351605391971, -1.4487804307455092),
vec2(-1.9846534175773436, 3.221269101027444),
vec2(-0.40124365935899264, 2.1045850223176945),
vec2(4.046893032549321, 2.0392476764653242),
vec2(3.4559105735133024, -0.3807816858860098),
vec2(-0.6158211813384815, -2.1316452969624353),
vec2(0.8262901560278462, 3.8513520732716535),
vec2(-3.397070108883404, 0.1591000340513467),
vec2(2.6032831636307425, 1.0972110152523402),
vec2(-3.5231375915307384, 3.0920695672856686),
vec2(4.14522537972708, -1.9083031868798375),
vec2(-3.416443787733975, -0.27138605069762517),
vec2(-1.709920014201684, 2.653846968185382),
vec2(-2.3877149029483364, 0.37952284287487087),
vec2(-4.0825777940951085, 0.15802117952020917),
vec2(2.4422644926102843, -1.9419546162288428),
vec2(-2.0331834197837595, -1.56307652506401),
vec2(-0.4469473423399445, 1.8231786744610559),
vec2(1.384283941770678, 1.7892846137503868),
vec2(-0.48913978455302065, -0.14981794269038773),
vec2(1.1174245088572805, 1.2366659153447346),
vec2(-1.6425944901938214, 0.21762212022076247),
vec2(-2.849873650494263, 2.7826533370379853),
vec2(-2.6381954805754058, -3.540821222907937),
vec2(2.5051843229276654, 2.338704298506849),
vec2(-3.3003454267988843, -3.511626210098056),
vec2(0.36333007049528415, -2.634064133741344),
vec2(0.6269225369003943, 0.25328340602731164),
vec2(0.20726945049042605, 3.8879548892736575),
vec2(-1.4702055964967795, -0.23468450242544606),
vec2(-3.956462860093185, -2.4548705082106888),
vec2(-2.716871595177548, -0.3283765320627414),
vec2(0.4142573027346813, 4.110475828269008),
vec2(2.0822603919165394, 3.165437872437243),
vec2(1.414371443343098, -4.3845709368105785),
vec2(1.444394562004221, -0.5787356180341101),
vec2(2.184137168841759, -3.9031335490326886),
vec2(-3.1879958947837204, -3.079472100437595),
vec2(2.7544392197787375, -2.1606313979085057),
vec2(3.261802532793593, 3.20235993515937),
vec2(-0.21071104297198645, 0.9153443411013353),
vec2(-1.2950588048149192, 2.6689002142427984),
vec2(4.535168824624113, -1.7165345219562858),
vec2(2.8312529944071354, 0.28593578434035194),
vec2(0.5121261242846467, 4.374539832085179),
vec2(-0.5476265200161429, -0.9543973115929132),
vec2(-4.940605792352427, -0.5209515530082043),
vec2(0.05471581553238847, -1.5947153786523631),
vec2(3.732314779297549, 0.4345817814554558),
vec2(-0.7201009112060947, 2.892419313106231),
vec2(-1.9420747734230772, 0.11976793199727923),
vec2(0.9698966590951398, 2.395641502079045),
vec2(-1.0893453353344484, -0.7928009718939866),
vec2(-3.4624726403747617, -3.0040040779594226),
vec2(-1.3978015296125441, 0.09223505931094887),
vec2(1.9346040958324107, 2.789679616777763),
vec2(-3.2572671716384445, -1.8075888812079621),
vec2(1.7577824181488217, -1.8105668350274533),
vec2(-3.637852700263026, -0.8510058227121616),
vec2(0.17390819124395188, 3.6162955772285645),
vec2(1.4728094698389778, 0.9558085202315721),
vec2(-0.6044273065071091, -2.5713750797082904),
vec2(0.09150012702716538, 3.894260702527247),
vec2(-3.273238927234555, 0.2650555134295571),


    };

    float slope = slopeBias ? (a / d) * scale : 0;
    bias *= scale;
//    float slope = slopeBias ? (a / d) : 0;


    // If there are color transitions between cascades disable it is caused by self shadowing before changing the values disable the filtering and just use one shadow map sample per pixel.

    if (doFiltering == false)
    {
        float closestDeptha = texture2D(shadowMaps[layer], posLightSpace.xy).r;
        return (currentDepth - bias - slope * slopeBiasScale) > closestDeptha ? 1.0 : 0.0; 
    }

    vec2 texelSize = 1.0 / vec2(textureSize(shadowMaps[layer], 0));
    float shadow = 0.0;
    scale = 1.0 / scale;
    for (int i = 0; i < samples.length(); i++)
    {
        float closestDepth = texture2D(shadowMaps[layer], posLightSpace.xy + samples[i] * filteringRadius * (scale) * texelSize).r;
        shadow += (currentDepth - bias - slope * slopeBiasScale) > closestDepth ? 1.0 : 0.0;        
    }
    return shadow / 81.0;
    // Average texels around the point. This is also called percentage-closer filtering.

    //
    //vec2 texelSize = 1.0 / vec2(textureSize(shadowMaps[layer], 0));
    //float shadow = 0.0;
    //for (int x = -1; x <= 1; x++)
    //{
    //    for (int y = -1; y <= 1; y++)
    //    {
    //        float closestDepth = texture2D(shadowMaps[layer], posLightSpace.xy + vec2(x, y) * texelSize).r;
    //        shadow += (currentDepth - bias - slope * slopeBiasScale) > closestDepth ? 1.0 : 0.0;        
    //    }    
    //}
    //shadow /= 9.0;
    //return shadow;
}

  vec2 samples[] = {
vec2(-0.9807698146299257, 3.13771918060967),
vec2(-3.7593829496620748, -0.6011227929995799),
vec2(2.263746874387353, 2.813703869529519),
vec2(-0.07386088891488883, 1.6686237210665213),
vec2(-3.4340075881475824, -3.0544529503366906),
vec2(3.6658549603624353, -1.1815130963435525),
vec2(0.751675248090715, -1.0104349165040476),
vec2(-1.4037455862230914, 1.3226949675256072),
vec2(1.5451384082974335, 0.5453050972756344),
vec2(1.4556014253933187, -1.3056838582605976),
vec2(2.7109985845547535, -3.093037807953159),
vec2(0.702893224701851, -2.0292067224200827),
vec2(-4.100462489447312, -1.1714061455673943),
vec2(3.241102706241288, 2.336709964639262),
vec2(1.8716801817642352, -2.9876120004239364),
vec2(-1.6816910054801781, 1.039266190244359),
vec2(2.816400543160108, -3.08178162801233),
vec2(0.7746351605391971, -1.4487804307455092),
vec2(-1.9846534175773436, 3.221269101027444),
vec2(-0.40124365935899264, 2.1045850223176945),
vec2(4.046893032549321, 2.0392476764653242),
vec2(3.4559105735133024, -0.3807816858860098),
vec2(-0.6158211813384815, -2.1316452969624353),
vec2(0.8262901560278462, 3.8513520732716535),
vec2(-3.397070108883404, 0.1591000340513467),
vec2(2.6032831636307425, 1.0972110152523402),
vec2(-3.5231375915307384, 3.0920695672856686),
vec2(4.14522537972708, -1.9083031868798375),
vec2(-3.416443787733975, -0.27138605069762517),
vec2(-1.709920014201684, 2.653846968185382),
vec2(-2.3877149029483364, 0.37952284287487087),
vec2(-4.0825777940951085, 0.15802117952020917),
vec2(2.4422644926102843, -1.9419546162288428),
vec2(-2.0331834197837595, -1.56307652506401),
vec2(-0.4469473423399445, 1.8231786744610559),
vec2(1.384283941770678, 1.7892846137503868),
vec2(-0.48913978455302065, -0.14981794269038773),
vec2(1.1174245088572805, 1.2366659153447346),
vec2(-1.6425944901938214, 0.21762212022076247),
vec2(-2.849873650494263, 2.7826533370379853),
vec2(-2.6381954805754058, -3.540821222907937),
vec2(2.5051843229276654, 2.338704298506849),
vec2(-3.3003454267988843, -3.511626210098056),
vec2(0.36333007049528415, -2.634064133741344),
vec2(0.6269225369003943, 0.25328340602731164),
vec2(0.20726945049042605, 3.8879548892736575),
vec2(-1.4702055964967795, -0.23468450242544606),
vec2(-3.956462860093185, -2.4548705082106888),
vec2(-2.716871595177548, -0.3283765320627414),
vec2(0.4142573027346813, 4.110475828269008),
vec2(2.0822603919165394, 3.165437872437243),
vec2(1.414371443343098, -4.3845709368105785),
vec2(1.444394562004221, -0.5787356180341101),
vec2(2.184137168841759, -3.9031335490326886),
vec2(-3.1879958947837204, -3.079472100437595),
vec2(2.7544392197787375, -2.1606313979085057),
vec2(3.261802532793593, 3.20235993515937),
vec2(-0.21071104297198645, 0.9153443411013353),
vec2(-1.2950588048149192, 2.6689002142427984),
vec2(4.535168824624113, -1.7165345219562858),
vec2(2.8312529944071354, 0.28593578434035194),
vec2(0.5121261242846467, 4.374539832085179),
vec2(-0.5476265200161429, -0.9543973115929132),
vec2(-4.940605792352427, -0.5209515530082043),
vec2(0.05471581553238847, -1.5947153786523631),
vec2(3.732314779297549, 0.4345817814554558),
vec2(-0.7201009112060947, 2.892419313106231),
vec2(-1.9420747734230772, 0.11976793199727923),
vec2(0.9698966590951398, 2.395641502079045),
vec2(-1.0893453353344484, -0.7928009718939866),
vec2(-3.4624726403747617, -3.0040040779594226),
vec2(-1.3978015296125441, 0.09223505931094887),
vec2(1.9346040958324107, 2.789679616777763),
vec2(-3.2572671716384445, -1.8075888812079621),
vec2(1.7577824181488217, -1.8105668350274533),
vec2(-3.637852700263026, -0.8510058227121616),
vec2(0.17390819124395188, 3.6162955772285645),
vec2(1.4728094698389778, 0.9558085202315721),
vec2(-0.6044273065071091, -2.5713750797082904),
vec2(0.09150012702716538, 3.894260702527247),
vec2(-3.273238927234555, 0.2650555134295571),


    };

float shadow(vec3 posWorldSpace, vec3 normal)
{
    posWorldSpace -= directionalLightDir * 0.01;
    vec4 fragPosViewSpace = worldToView * vec4(posWorldSpace, 1.0);
    // This value should be negated or not based on the sign of the forward vector.
    // Just using abs makes it simpler becuase it works with any forward direction.
    float distanceFromView = abs(fragPosViewSpace.z);

    int layer = -1;
    for (int i = 0; i < cascadeCount; ++i)
    {
        if (distanceFromView < cascadeZ[i])
        {
            layer = i;
            break;
        }
    }
    if (layer == -1)
    {
        layer = cascadeCount;
    }

    float scale;

    float biasModifier = 0.5;
    if (useScaleAabb)
    {
       scale = cascadeScale[layer].z;
    }
    else
    {
        if (layer == cascadeCount)
        {
            scale = 1 / (farPlane * biasModifier);
        }
        else
        {
            scale = 1 / (cascadeZ[layer] * biasModifier);
        }
    }
    scale = 1.0f / (scale * scaleScale);

    float d = dot(normal, directionalLightDir);
    float a = sqrt(1 - d * d);
    if (normalBias)
    {
        posWorldSpace += normal * a * normalBiasScale;
    }

    vec3 norm = normalize((transpose(inverse(worldToShadowMap[layer])) * vec4(normal, 0)).xyz);
    vec3 up = vec3(0, 1, 0);
    vec3 z = normalize(norm - dot(up, norm) * up);
    vec3 g = cross(up, z);

    vec3 posLightSpace = (worldToShadowMap[layer] * vec4(posWorldSpace, 1.0)).xyz;

    posLightSpace = posLightSpace * 0.5 + 0.5;

    float currentDepth = posLightSpace.z;

    if (currentDepth > 1.0)
    {
        return 0.0;
    }

    float bias = constantBias;

    float slope = slopeBias ? tan(acos(dot(normal, directionalLightDir))) : 0;
    slope *= slopeBiasScale;

    if (doFiltering == false)
    {
        float closestDeptha = texture2D(shadowMaps[layer], posLightSpace.xy).r;
        return (currentDepth - bias - slope) > closestDeptha ? 1.0 : 0.0; 
    }

    vec2 texelSize = 1.0 / vec2(textureSize(shadowMaps[layer], 0));
    float shadow = 0.0;
    scale = 1.0 / (scale);
    for (int i = 0; i < samples.length(); i++)
    {
//        float closestDepth = texture2D(shadowMaps[layer], posLightSpace.xy + samples[i] * scale * 0.05 * texelSize).r;
//        float closestDepth = texture2D(shadowMaps[layer], posLightSpace.xy + samples[i] * cascadeScale[layer].x * 0.05 * texelSize).r;
float closestDepth = texture2D(shadowMaps[layer], posLightSpace.xy + samples[i] * cascadeScale[layer].x * 20  * texelSize).r;
        shadow += (currentDepth - bias - slope) > closestDepth ? 1.0 : 0.0;        
    } 
    return shadow / 81.0;

}

uniform sampler2D geometryPosWorld;
uniform sampler2D geometryAlbedo;
uniform sampler2D geometryNormal;
uniform sampler2D geometryDepth;

in vec2 texturePos;

out vec4 outColor;

void main() 
{
    vec3 color = texture2D(geometryAlbedo, texturePos).rgb;
    vec3 posWorld = texture2D(geometryPosWorld, texturePos).xyz;
    vec3 normal = normalize(texture(geometryNormal, texturePos).xyz); 
    float depth = texture2D(geometryDepth, texturePos).r;

    float shadow = shadow(posWorld, normal);
    float ambient = 0.3;
    float diffuse = clamp(dot(normal, -directionalLightDir), 0, 1);
    float specular = clamp(pow(dot(reflect(-directionalLightDir, normal), normal), 1), 0, 1);
    color *= ambient + ((1 - shadow) * diffuse);
    outColor = vec4(color, 1);
    gl_FragDepth = depth;
}