package
{
	import away3d.containers.View3D;
	import away3d.entities.Mesh;
	import away3d.events.AssetEvent;
	import away3d.events.LoaderEvent;
	import away3d.library.assets.AssetType;
	import away3d.loaders.Loader3D;
	import away3d.loaders.misc.AssetLoaderContext;
	import away3d.loaders.parsers.AWD2Parser;
	import away3d.materials.ColorMaterial;
	import away3d.materials.TextureMaterial;
	
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.geom.Vector3D;
	import flash.net.URLRequest;
	
	[SWF(width="800", height="600")]
	public class AWDViewer extends Sprite
	{
		private var _view : View3D;
		private var _loader : Loader3D;
		
		public function AWDViewer()
		{
			_view = new View3D();
			_view.antiAlias = 4;
			addChild(_view);
			addEventListener(Event.ENTER_FRAME, onEnterFrame);
			
			load(loaderInfo.parameters['awd'] || 'default.awd');
		}
		
		
		private function load(url : String) : void
		{
			Loader3D.enableParser(AWD2Parser);
			_loader = new Loader3D(false);
			_loader.addEventListener(AssetEvent.ASSET_COMPLETE, onAssetComplete);
			_loader.addEventListener(LoaderEvent.RESOURCE_COMPLETE, onResourceComplete);
			_loader.load(new URLRequest(url), new AssetLoaderContext(false));
		}
		
		
		private function onAssetComplete(ev : AssetEvent) : void
		{
			if (ev.asset.assetType == AssetType.MESH) {
				var mesh : Mesh;
				var texMat : TextureMaterial;
				
				mesh = Mesh(ev.asset);
				texMat = mesh.material as TextureMaterial;
				
				if (mesh.material == null || (texMat && texMat.texture == null))
					mesh.material = new ColorMaterial(Math.random() * 0xffffff);
			}
		}
		
		
		private function onResourceComplete(ev : LoaderEvent) : void
		{
			var scale : Number;
			var max : Vector3D;
			var min : Vector3D;
			var d : Vector3D;
			
			max = new Vector3D(_loader.maxX, _loader.maxY, _loader.maxZ);
			min = new Vector3D(_loader.minX, _loader.minY, _loader.minZ);
			d = max.subtract(min);
			
			scale = 300 / d.length;
			_loader.scale(scale);
			
			_view.scene.addChild(_loader);
		}
		
		
		private function onEnterFrame(ev : Event) : void
		{
			_view.camera.y = stage.mouseY - stage.stageHeight/2;
			_view.camera.x = stage.mouseX - stage.stageWidth/2;
			_view.camera.lookAt(_loader.position);
			_view.render();
		}
	}
}