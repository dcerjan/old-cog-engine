import bpy
import json

class CogVertex:
    position = 0            # index of position vector3
    normal = 0              # intex of normal vector3
    tangent = 0             # index of tangent vector3
    texCoord = 0            # index of texCoord vector2
    bones = [0, 0, 0, 0]    # indices of affecting bones
    weights = [0, 0, 0, 0]  # weights of coresponding bones 0-255 range
    
class CogPolygon:
    vertices = [0, 0, 0]    # indices of coresponding vertices
    normal = 0              # index of normal vector3
    tangent = 0             # index of tangent vector3
    
class CogBone:
    name = ""                       # bone name
    parent = 0                      # index of parent bone
    position = [0.0, 0.0, 0.0]      # vector3 position in bone-local space
    rotation = [0.0, 0.0, 0.0, 1.0] # quaternion rotation in bone-local space
    scale = [1.0, 1.0, 1.0]         # vector3 scale in bone-local space
    
class CogKeyframe:
    time = 0.0              # normalized time (in [0.0, 1.0]) of curent keyframe
    transforms = {
        "bones": [],        # indices of bones
        "rotation": [],     # rotations of those bones
        "position": [],     # positions of those bones
        "scale": []         # scales of those bones
    }
    
class CogAnimation:
    name = ""               # name of animation
    keyframes = []          # list of keyframes

class CogMesh(object):
    
    data = {
        "name": "",
        "data": {
            "vertices": [],
            "polygons": [],
            "uvs": []
        },
        "material": {
            "diffuse": {
                "color": [1.0, 1.0, 1.0, 1.0],
                "intesity": 1.0,
                "texture": ""
            },
            "specular": {
                "color": [1.0, 1.0, 1.0, 1.0],
                "intesity": 1.0,
                "hardness": 50,
                "texture": ""
            },
            "ambient": {
                "ambientOcclusionMap": "",
                "ambientOcclusionStrenght": 1.0,
                "ambientIntensity": 1.0,
                "ambientColor": [1.0, 1.0, 1.0, 1.0]
            },
            "bump": {
                "normalMap": "",
                "heightMap": "",
                "heightScale": 1.0
            },
            "reflection": {
                "reflectionMap": "",
                "reflectionStrenght": 1.0,
                "reflectionColor": [1.0, 1.0, 1.0, 1.0]
            },
            "detail": {
                "detailMap": "",
                "detailStrenght": 1.0
            },
            "glow": {
                "glowMap": "",
                "glowStrenght": 1.0,
                "glowColor": [1.0, 1.0, 1.0, 1.0]
            }
        },
        "skeleton": {
            "bones": [],
            "animations": []
        }
    }
    
    def __init__(self):
        pass
    
    def write(self, filename):
        json.dump(self.data, filename)


def writeCogMesh(context, filepath, options):
    print("Exporting to CogMesh...")
    
    msh = CogMesh()
    
    bmsh = bpy.context.scene.objects.active
    
    msh.data["name"] = bmsh.name
    verts = msh.data["data"]["vertices"]
    polys = msh.data["data"]["polygons"]
    uvs = msh.data["data"]["uvs"]
    
    for uv in bmsh.data.uv_layers[0].data:
        uvs.append([uv.uv[0], 1.0 - uv.uv[1]])
    
    i = 0
    for v in bmsh.data.vertices:
        vert = {
            "position": [v.co[0], v.co[2], -v.co[1]],
            "normal": [v.normal[0], v.normal[2], -v.normal[1]]
        }
        i += 1
        verts.append(vert)
        
    i = 0
    for p in bmsh.data.polygons:
        poly = {
            "vertices": [p.vertices[0], p.vertices[1], p.vertices[2]],
            "normal": [p.normal[0], p.normal[1], p.normal[2]],
            "uvs": [i, i + 1, i + 2]
        }
        i += 3
        polys.append(poly)
            
    
    with open(filepath, "w") as out:
        msh.write(out)

    return {'FINISHED'}


from bpy_extras.io_utils import ExportHelper
from bpy.props import StringProperty, BoolProperty, EnumProperty
from bpy.types import Operator

class ExportCogMesh(Operator, ExportHelper):
    """Exports selected belnder mesh to cogmesh format"""
    bl_idname = "export_test.some_data"  # important since its how bpy.ops.import_test.some_data is constructed
    bl_label = "Export Cog Mesh"

    filename_ext = ".json"

    filter_glob = StringProperty(
            default="*.json",
            options={'HIDDEN'},
            )

    exportNormals = BoolProperty(name="Export Normals", description="Export normals", default=True)
    exportTangents = BoolProperty(name="Export Tangents", description="Export tangents", default=True)
    exportSkeleton = BoolProperty(name="Export Skeleton", description="Export skeleton data alongside mesh data", default=False)
    exportMaterial = BoolProperty(name="Export Material", description="Export material data alongside mesh data", default=False)

    #type = EnumProperty(
    #        name="Example Enum",
    #        description="Choose between two items",
    #        items=(('OPT_A', "First Option", "Description one"),
    #               ('OPT_B', "Second Option", "Description two")),
    #        default='OPT_A',
    #        )

    def execute(self, context):
        opts = {
            "normals": self.exportNormals, 
            "tangents": self.exportTangents,
            "skeleton": self.exportSkeleton,
            "material": self.exportMaterial
        }
        return writeCogMesh(context, self.filepath, opts)


def menu_func_export(self, context):
    self.layout.operator(ExportCogMesh.bl_idname, text="CogMesh Export Operator")


def register():
    bpy.utils.register_class(ExportCogMesh)
    bpy.types.INFO_MT_file_export.append(menu_func_export)


def unregister():
    bpy.utils.unregister_class(ExportSomeData)
    bpy.types.INFO_MT_file_export.remove(menu_func_export)

if __name__ == "__main__":
    register()

    # test call
    bpy.ops.export_test.some_data('INVOKE_DEFAULT')
