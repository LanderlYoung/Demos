use tauri::{LogicalPosition, LogicalSize, Manager, WebviewBuilder, WebviewUrl, WindowBuilder};

// Learn more about Tauri commands at https://tauri.app/v1/guides/features/command
#[tauri::command]
fn greet(name: &str) -> String {
    format!("Hello, {}! You've been greeted from Rust!", name)
}

#[cfg_attr(mobile, tauri::mobile_entry_point)]
pub fn run() {
    tauri::Builder::default()
        .plugin(tauri_plugin_notification::init())
        .plugin(tauri_plugin_dialog::init())
        .plugin(tauri_plugin_shell::init())
        .invoke_handler(tauri::generate_handler![greet, open_new_window])
        .run(tauri::generate_context!())
        .expect("error while running tauri application");
}

#[tauri::command]
fn open_new_window(app_handle: tauri::AppHandle, label: &str, url: &str) {
    // https://docs.rs/tauri/2.0.0-rc/tauri/window/struct.WindowBuilder.html

    match app_handle.get_window(label) {
        None => {
            let width = 1280.;
            let height = 800.;
            let webview_builder =
                WebviewBuilder::new(label, WebviewUrl::External(url.parse().unwrap()))
                    .auto_resize();

            let window_builder =
                WindowBuilder::new(&app_handle, label).title("secondary window from rust");

            let window = window_builder.build().unwrap();
            let _webview = window
                .add_child(
                    webview_builder,
                    LogicalPosition::new(0., 0.),
                    LogicalSize::new(width, height),
                )
                .unwrap();
        }
        Some(window) => {
            window.show().unwrap();
        }
    }
}
