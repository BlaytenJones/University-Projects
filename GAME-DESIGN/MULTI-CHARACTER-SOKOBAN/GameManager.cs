using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;
using UnityEngine.SceneManagement;

public class GameManager : MonoBehaviour
{
    public static GameManager Instance { get; private set; }
    public GameObject tilePrefab;
    public GameObject echoPrefab;
    public GameObject scientistPrefab;
    public bool inCutscene = false;
    public bool cutscenePlayed = false;
    [System.NonSerialized]
    public int currLevel = 0;
    [SerializeField]
    public List<KeyCode> lastInput = new();
    [System.NonSerialized]
    public bool newMove = false;
    [System.NonSerialized]
    public bool loseCondition = false;
    [System.NonSerialized]
    //The win condition must be 2 for the level to change
    public int winCondition = 0;
    [System.NonSerialized]
    public bool buttCondition = false;
    [System.NonSerialized]
    public bool start = false;
    [System.NonSerialized]
    public int numInitialized = 0;
    public int turnCount = 0;
    //private readonly string levelFolderPath = "Assets/Level_Info/";
    public string[] levelFiles = new string[4];
    private void Awake() {
        if(Instance != null && Instance != this) {
            Destroy(gameObject);
        } else {
            Instance = this;
            DontDestroyOnLoad(gameObject);
        }
    }

    private void Update() {
        if(winCondition >= 2) {
            start = false;
            ChangeScene("Level " + (currLevel + 1));
            winCondition = 0;
            buttCondition = false;
            lastInput.Clear();
            turnCount = 0;
        }
        if(numInitialized >= 187) {
            start = true;
            numInitialized = 0;
        }
    }

    public void Parse() {
        currLevel = int.Parse(SceneManager.GetActiveScene().name[5..]);
        if (currLevel != 0) {
            //StreamReader reader = new(levelFolderPath + currLevel + ".txt");
            string text = levelFiles[currLevel - 1];
            int invalidCount = 0;
            for (int i = 0; i < text.Length; i++) {
                switch (text[i]) {
                    case 'X':
                        InstantiateNewTile(i, "Void", invalidCount);
                        break;
                    case 'O':
                        InstantiateNewTile(i, "Walkable", invalidCount);
                        break;
                    case 'W':
                        InstantiateNewTile(i, "Wall", invalidCount);
                        break;
                    case 'B':
                        InstantiateNewTile(i, "Button", invalidCount);
                        break;
                    case 'C':
                        GameObject tileHolder = GameObject.Find("Tile Holder");
                        GameObject newTile = Instantiate(tilePrefab);
                        newTile.transform.parent = tileHolder.transform;
                        newTile.tag = "Untagged";
                        newTile.transform.position = new Vector3(-16 + 2 * ((i - invalidCount) % 17), 0, 16 - 2 * (Mathf.Floor((i - invalidCount) / 17)));
                        newTile.GetComponent<TileTagTransform>().crate = true;
                        break;
                    case 'L':
                        InstantiateNewTile(i, "Laser", invalidCount);
                        break;
                    case 'n':
                        InstantiateNewTile(i, "Drone", invalidCount);
                        break;
                    case 'e':
                        InstantiateNewTile(i, "Drone", invalidCount, 90);
                        break;
                    case 's':
                        InstantiateNewTile(i, "Drone", invalidCount, 180);
                        break;
                    case 'w':
                        InstantiateNewTile(i, "Drone", invalidCount, 270);
                        break;
                    case 'E':
                        InstantiateNewTile(i, "Exit", invalidCount);
                        break;
                    case '1':
                        InstantiateNewTile(i, "Start", invalidCount);
                        GameObject echo = Instantiate(echoPrefab);
                        echo.transform.position = new Vector3(-16 + 2 * ((i - invalidCount) % 17), 1, 16 - 2 * (Mathf.Floor((i - invalidCount) / 17)));
                        break;
                    case '2':
                        InstantiateNewTile(i, "Start", invalidCount);
                        GameObject scientist = Instantiate(scientistPrefab);
                        scientist.transform.position = new Vector3(-16 + 2 * ((i - invalidCount) % 17), 1, 16 - 2 * (Mathf.Floor((i - invalidCount) / 17)));
                        break;
                    default:
                        invalidCount++;
                        break;
                }
            }
        }
    }

    public void ChangeScene(string sceneName) {
        cutscenePlayed = false;
        SceneManager.LoadScene(sceneName);
    }

    private void InstantiateNewTile(int index, string tag, int invalidCount, int rotation = 0) {
        GameObject tileHolder = GameObject.Find("Tile Holder");
        GameObject newTile = Instantiate(tilePrefab);
        newTile.transform.parent = tileHolder.transform;
        newTile.transform.localRotation = Quaternion.Euler(0, rotation, 0);
        newTile.tag = tag;
        newTile.transform.position = new Vector3(-16 + 2 * ((index - invalidCount) % 17), 0, 16 - 2 * (Mathf.Floor((index - invalidCount) / 17)));
    }
}
