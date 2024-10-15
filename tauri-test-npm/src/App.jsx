import { useState } from "react";
import reactLogo from "./assets/react.svg";
import { invoke } from "@tauri-apps/api/core";
import { message, } from '@tauri-apps/plugin-dialog';
import { Window } from "@tauri-apps/api/window"
import { Webview } from "@tauri-apps/api/webview"
import { isPermissionGranted, requestPermission, sendNotification } from '@tauri-apps/plugin-notification';
import "./App.css";

function App() {
  const [greetMsg, setGreetMsg] = useState("");
  const [name, setName] = useState("");

  async function greet() {
    // Learn more about Tauri commands at https://tauri.app/v1/guides/features/command
    const msg = await invoke("greet", { name });
    setGreetMsg(msg);
    await message(msg, {
      title: 'Tauri Dialog Test',
      kind: 'warning',
      okLabel: 'Great'
    });
  }

  return (
    <div className="container">
      <h1>Welcome to Tauri!</h1>

      <div className="row">
        <a href="https://vitejs.dev" target="_blank">
          <img src="/vite.svg" className="logo vite" alt="Vite logo" />
        </a>
        <p href="https://tauri.app" target="_blank" onClick={(e) => {
          e.preventDefault();
          openNewWindow('tauri_webview', "https://reactjs.org")
        }}>
          <img src="/tauri.svg" className="logo tauri" alt="Tauri logo" />
        </p>
        <a href="https://reactjs.org" target="_blank">
          <img src={reactLogo} className="logo react" alt="React logo" />
        </a>
      </div>

      <p>Click on the Tauri, Vite, and React logos to learn more.</p>

      <form
        className="row"
        onSubmit={(e) => {
          e.preventDefault();
          greet();
        }}
      >
        <input
          id="greet-input"
          onChange={(e) => setName(e.currentTarget.value)}
          placeholder="Enter a name..."
        />
        <button type="submit">Greet</button>
      </form>

      <p>{greetMsg}</p>
    </div>
  );
}

async function logEvent(who, name) {
  await who.once(name, e => {
    console.log(`${name} event: ${JSON.stringify(e)}`);
  });
}

async function openNewWindow(label, url) {
  // TODO: tauri window/webview JS API not working properly
  // https://tauri.app/reference/javascript/api/namespacewebview/
  console.log("open webview " + url);

  await postNotification("open webview " + url);

  const appWindow = new Window(label);
  const webview = new Webview(appWindow, label, {
    url: url,
    x: 0,
    y: 0,
    width: 800,
    height: 600,
  })
  await logEvent(webview, 'tauri://created');
  await logEvent(webview, 'tauri://error');
  console.log("created webview " + webview + " for url " + url);

  appWindow.once('tauri://created', e => {
    appWindow.setTitle('Test second window');
    setTimeout(() => {
      console.log("close window " + appWindow.label + " for url " + url);
      appWindow.close();
    }, 5000);
  })
}

let needRequestPermission = true;
async function postNotification(body, title) {
  let permissionGranted = await isPermissionGranted();
  if (!permissionGranted && needRequestPermission) {
    needRequestPermission = false;
    const permission = await requestPermission();
    permissionGranted = permission === 'granted';
  }
  if (permissionGranted) {
    sendNotification({ title: title, body: body });
  }
}

export default App;
